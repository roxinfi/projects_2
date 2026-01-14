#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "driver/i2c.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

static const char *TAG = "wifi_sender";

// Wi-Fi STA
#define WIFI_SSID        "ESP32_AP"
#define WIFI_PASS        "12345678"
#define WIFI_CONNECTED   BIT0
static EventGroupHandle_t wifi_event_group;

// TCP target
#define HOST_IP          "192.168.4.1"
#define HOST_PORT        1234
#define SEND_INTERVAL_MS 5000

// I2C/BMP280
#define I2C_NUM         I2C_NUM_0
#define I2C_SDA_GPIO    21
#define I2C_SCL_GPIO    22
#define BMP280_ADDR     0x76

// Calibration
static uint16_t dig_T1;
static int16_t  dig_T2, dig_T3;
static int32_t  t_fine;

// ——— I2C helpers ——————————————————————————————————————————————
static esp_err_t i2c_read(uint8_t reg, uint8_t *buf, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (BMP280_ADDR<<1)|I2C_MASTER_WRITE, true);
      i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (BMP280_ADDR<<1)|I2C_MASTER_READ, true);
      i2c_master_read(cmd, buf, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}
static esp_err_t i2c_write(uint8_t reg, uint8_t val) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (BMP280_ADDR<<1)|I2C_MASTER_WRITE, true);
      i2c_master_write_byte(cmd, reg, true);
      i2c_master_write_byte(cmd, val, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// ——— BMP280 init & comp. ———————————————————————————————————————
static void bmp280_read_cal() {
    uint8_t buf[6];
    i2c_read(0x88, buf, 6);
    dig_T1 = (buf[1]<<8)|buf[0];
    dig_T2 = (buf[3]<<8)|buf[2];
    dig_T3 = (buf[5]<<8)|buf[4];
}
static void bmp280_init() {
    i2c_write(0xE0, 0xB6);           // reset
    vTaskDelay(pdMS_TO_TICKS(100));
    bmp280_read_cal();
    i2c_write(0xF4, 0x27);           // osrs_t x1, normal mode
    i2c_write(0xF5, 0xA0);           // config standby 500ms
}
static int32_t bmp280_rawtemp() {
    uint8_t buf[3];
    i2c_read(0xFA, buf, 3);
    return ((int32_t)buf[0]<<12)|((int32_t)buf[1]<<4)|(buf[2]>>4);
}
static float bmp280_comp_temp(int32_t adc_T) {
    int32_t var1 = (((adc_T>>3) - ((int32_t)dig_T1<<1)) * (int32_t)dig_T2) >> 11;
    int32_t var2 = (((((adc_T>>4) - (int32_t)dig_T1) * ((adc_T>>4) - (int32_t)dig_T1))>>12)
                    *(int32_t)dig_T3) >> 14;
    t_fine = var1 + var2;
    float T = (t_fine * 5 + 128) >> 8;
    return T/100.0f;
}

// ——— I2C master init ——————————————————————————————————————————
static void i2c_master_init() {
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    i2c_param_config(I2C_NUM, &cfg);
    i2c_driver_install(I2C_NUM, cfg.mode, 0, 0, 0);
}

// ——— Wi-Fi event handler ——————————————————————————————————————
static void wifi_event(void* _, esp_event_base_t b, int32_t id, void* d) {
    if (b==WIFI_EVENT && id==WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (b==WIFI_EVENT && id==WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
    } else if (b==IP_EVENT && id==IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED);
    }
}

// ——— Wi-Fi STA init ——————————————————————————————————————————
static void wifi_init_sta() {
    wifi_event_group = xEventGroupCreate();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wcfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wcfg);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event, NULL);

    wifi_config_t sta = {
        .sta = {.ssid = WIFI_SSID, .password = WIFI_PASS,
                .threshold.authmode = WIFI_AUTH_WPA2_PSK},
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &sta);
    esp_wifi_start();
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED, false, true, portMAX_DELAY);
}

// ——— TCP client task —————————————————————————————————————————
static void tcp_client_task(void* pv) {
    char buf[64];
    while (1) {
        int32_t raw = bmp280_rawtemp();
        float temp = bmp280_comp_temp(raw);
        int len = snprintf(buf, sizeof(buf), "Temp: %.1f C\n", temp);

        struct sockaddr_in dst = {
            .sin_family = AF_INET,
            .sin_port   = htons(HOST_PORT),
            .sin_addr.s_addr = inet_addr(HOST_IP),
        };

        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s>=0 && connect(s, (struct sockaddr*)&dst, sizeof(dst))==0) {
            send(s, buf, len, 0);
            close(s);
        }
        vTaskDelay(pdMS_TO_TICKS(SEND_INTERVAL_MS));
    }
}

void app_main(void) {
    nvs_flash_init();
    i2c_master_init();
    bmp280_init();
    wifi_init_sta();
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
}
