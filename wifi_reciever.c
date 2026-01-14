/*=============================================================================
    File Name: wifi_sender_receiver.c  
    Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
    Date: 15/06/2025
    Modified: None
    © Fanshawe College, 2025

    Description: This code implements a Wi-Fi sender and receiver using ESP-IDF. 
    The sender connects to a Wi-Fi network and sends messages to a server at regular intervals.
    The receiver sets up a Wi-Fi access point and listens for incoming TCP connections,
    receiving messages from clients.
=============================================================================*/


#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

// Global variables
static const char *TAG = "wifi_test";

// Wi-Fi configuration
#define WIFI_SSID     "ESP32_AP"
#define WIFI_PASS     "12345678"
#define PORT          1234
#define WIFI_CONNECTED_BIT BIT0
static EventGroupHandle_t wifi_event_group;

// ================= Sender Code =====================
#define HOST_IP "192.168.4.1"
#define SEND_INTERVAL_MS 5000

/*>>>wifi_event_handler: =========================================================
Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date: 15/06/2025
Modified: None
Desc: Handles Wi-Fi events such as connection start, disconnection, and IP acquisition.
Input: arg (void*), event_base (esp_event_base_t), event_id (int32_t), event_data (void*)
Return: None
==============================================================================
*/
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) 
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        esp_wifi_connect();
    }
     else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}// eo wifi_event_handler::

/* >>> wifi_init_sta: ============================================================
Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date: 15/06/2025
Modified: None
Desc: Initializes the Wi-Fi station mode, sets up event handlers, and connects to the specified Wi-Fi network.
Input: None
Return: None
==============================================================================
*/
static void wifi_init_sta() 
{
    wifi_event_group = xEventGroupCreate();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t sta_config = 
    {
        .sta = 
        {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &sta_config);
    esp_wifi_start();

    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
}// eo wifi_init_sta::


/* >>> tcp_client_task: ==========================================================
Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date: 15/06/2025
Modified: None
Desc: Task that creates a TCP client socket, connects to the server, and sends a message
Input: pv (void*)
Return: None
==============================================================================
*/
static void tcp_client_task(void* pv) 
{
    char message[] = "ESP32 client connected!\n";
    while (1) 
    {
        struct sockaddr_in dest_addr = 
        {
            .sin_family = AF_INET,
            .sin_port = htons(PORT),
            .sin_addr.s_addr = inet_addr(HOST_IP),
        };
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock >= 0 && connect(sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == 0) 
        {
            send(sock, message, strlen(message), 0);
            close(sock);
        }
        vTaskDelay(pdMS_TO_TICKS(SEND_INTERVAL_MS));
    }
}// eo tcp_client_task::

void app_main(void) 
{
    nvs_flash_init();
    wifi_init_sta();
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
} // eo app_main::

// ================= Receiver Code =====================

/* >>> wifi_init_ap: ============================================================
Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date: 15/06/2025
Modified: None
Desc: Initializes the Wi-Fi access point mode, sets up the AP configuration, and starts the Wi-Fi AP.
Input: None
*/
static void wifi_init_ap() 
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    wifi_config_t ap_config = 
    {
        .ap = 
        {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .password = WIFI_PASS,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .channel = 1,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &ap_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Access Point \"%s\" started. IP: 192.168.4.1", WIFI_SSID);
}// eo wifi_init_ap::

/* >>> tcp_server_task: ==========================================================
Author: Vraj Patel, Vamseedhar Reddy, Mihir Jariwala, Samip Patel
Date: 15/06/2025
Modified: None
Desc: Task that creates a TCP server socket, listens for incoming connections, and receives messages.
Input: _ (void*)
Return: None
==============================================================================
*/
static void tcp_server_task(void* _) 
{
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in6 server_addr = 
    {
        .sin6_family = AF_INET6,
        .sin6_port = htons(PORT),
        .sin6_addr = in6addr_any,
    };
    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 1);

    while (1) 
    {
        int client_sock = accept(server_sock, NULL, NULL);
        if (client_sock < 0) 
        { 
            vTaskDelay(pdMS_TO_TICKS(100)); continue; 
        }
        char buf[64];
        int len = recv(client_sock, buf, sizeof(buf) - 1, 0);
        close(client_sock);
        if (len > 0) 
        {
            buf[len] = 0;
            ESP_LOGI(TAG, "Received Message: %s", buf);
        }
    }
}// eo tcp_server_task::

void app_main(void) 
{
    nvs_flash_init();
    wifi_init_ap();
    xTaskCreate(tcp_server_task, "tcp_server", 4096, NULL, 5, NULL);
}// eo app_main::