/*=============================================================================
    File Name: BME280_temp_test.c
    Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
    Date: 18/06/2025
    Modified: None
    © Fanshawe College, 2025

    Description: This code initializes the BMP280 temperature sensor using I2C communication on an ESP32.
    It reads the raw temperature data, compensates it using calibration parameters, and logs the temperature in degrees Celsius.
    The I2C interface is configured with specified GPIO pins and frequency, and the sensor is set up to read temperature data at regular intervals.
    The temperature is logged every 5 seconds.
=============================================================================*/

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "BMP280_TERM";


// I2C configuration
#define I2C_PORT        I2C_NUM_0
#define I2C_SDA_GPIO    21
#define I2C_SCL_GPIO    22
#define I2C_FREQ_HZ     100000
#define BMP280_ADDR     0x76

// Calibration parameters
static uint16_t dig_T1;
static int16_t  dig_T2, dig_T3;
static int32_t  t_fine;



/*>>> esp_err_t i2c_read(uint8_t reg, uint8_t *buf, size_t len)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function reads data from the BMP280 sensor over I2C.
        It sends the register address to read from, then reads the specified number of bytes into the buffer.
Input: 		None 
Returns:	None
==============================================================================*/
// Low-level I2C read/write
static esp_err_t i2c_read(uint8_t reg, uint8_t *buf, size_t len) 
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (BMP280_ADDR << 1) | I2C_MASTER_WRITE, true);
      i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (BMP280_ADDR << 1) | I2C_MASTER_READ, true);
      i2c_master_read(cmd, buf, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}// eo i2c_read::

/*>>> esp_err_t i2c_write(uint8_t reg, uint8_t val)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function writes a single byte to a specified register of the BMP280 sensor over I2C.
        It sends the register address followed by the value to write.
Input: 		None
Returns:	None
==============================================================================
*/
static esp_err_t i2c_write(uint8_t reg, uint8_t val) 
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (BMP280_ADDR << 1) | I2C_MASTER_WRITE, true);
      i2c_master_write_byte(cmd, reg, true);
      i2c_master_write_byte(cmd, val, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}// eo i2c_write::

/*>>> i2c_master_init(void)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function initializes the I2C master interface with the specified SDA and SCL GPIO pins,
        sets the pull-up resistors, and configures the clock speed.
Input: 		None
Returns:	None
==============================================================================
*/
// Initialize I2C master and log it
static void i2c_master_init(void)
{
    i2c_config_t cfg = 
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_GPIO,
        .scl_io_num = I2C_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };
    i2c_param_config(I2C_PORT, &cfg);
    i2c_driver_install(I2C_PORT, cfg.mode, 0, 0, 0);
    ESP_LOGI(TAG, "I2C initialized on SDA=%d, SCL=%d", I2C_SDA_GPIO, I2C_SCL_GPIO);
}// eo i2c_master_init::

/*>>> bmp280_read_calibration(void)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function reads the calibration data from the BMP280 sensor.
        It reads 6 bytes from the calibration registers and stores them in global variables.
Input: 		None
Returns:	None
===============================================================================
*/
static void bmp280_read_calibration(void) 
{
    uint8_t buf[6];
    i2c_read(0x88, buf, sizeof(buf));
    dig_T1 = (buf[1] << 8) | buf[0];
    dig_T2 = (buf[3] << 8) | buf[2];
    dig_T3 = (buf[5] << 8) | buf[4];
    ESP_LOGI(TAG, "Calib T1=%u, T2=%d, T3=%d", dig_T1, dig_T2, dig_T3);
}// eo bmp280_read_calibration::

/*>>> bmp280_init(void)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function initializes the BMP280 sensor by performing a soft reset,
        reading the calibration data, and configuring the control registers for temperature measurement.
Input: 		None
Returns:	None
===============================================================================
*/
static void bmp280_init(void) 
{
    i2c_write(0xE0, 0xB6);                     // soft reset
    vTaskDelay(pdMS_TO_TICKS(100));            // wait for reset
    bmp280_read_calibration();                 // read & log T1–T3
    i2c_write(0xF4, 0x27);                     // ctrl_meas: osrs_t=1, normal mode
    i2c_write(0xF5, 0xA0);                     // config: standby=500ms, filter off
}// eo bmp280_init::

/*>>> bmp280_read_raw_temp(void)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function reads the raw temperature data from the BMP280 sensor.
        It reads 3 bytes from the temperature data registers and combines them into a single 20-bit value.
Input: 		None
Returns:	None
===============================================================================
*/ 
static int32_t bmp280_read_raw_temp(void) 
{
    uint8_t d[3];
    i2c_read(0xFA, d, 3);
    return ((int32_t)d[0] << 12) | ((int32_t)d[1] << 4) | (d[2] >> 4);
}// eo bmp280_read_raw_temp::

/*>>> bmp280_compensate_temp(int32_t adc_T)
Author:		Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date:		15/06/2025
Modified:	None
Desc:		 This function compensates the raw temperature value using the calibration data.
        It calculates the fine temperature value and returns the compensated temperature in degrees Celsius.
Input: 		adc_T (int32_t), the raw temperature value read from the sensor
Returns:	Compensated temperature in degrees Celsius (float)
===============================================================================
*/
static float bmp280_compensate_temp(int32_t adc_T) 
{
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * dig_T2) >> 11;
    int32_t var2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) * dig_T3) >> 14;
    t_fine = var1 + var2;
    int32_t T = (t_fine * 5 + 128) >> 8;
    return T / 100.0f;
}// eo bmp280_compensate_temp::


// ===========Main Function==========================
void app_main(void) 
{
    // init NVS (optional)
    ESP_ERROR_CHECK(nvs_flash_init());

    i2c_master_init();   // logs I2C init
    bmp280_init();       // soft reset + log calibration

    float temp = 23.45f;
    while (1) 
    {
        vTaskDelay(pdMS_TO_TICKS(5000));               // 5s interval
        int32_t raw = bmp280_read_raw_temp();
        temp = bmp280_compensate_temp(raw);
        ESP_LOGI(TAG, "Temperature: %.2f °C", temp);
    }
}// eo app_main::
