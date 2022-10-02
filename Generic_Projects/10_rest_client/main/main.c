/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "connect.h"
#include "cJSON.h"

#include "nvs_flash.h"
#include "esp_log.h"

#include "esp_http_client.h"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 2048

static const char *TAG = "REST";
esp_err_t on_client_data(esp_http_client_event_t *evt){
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read

    switch(evt->event_id){
        // Quando chega dados
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI("on_client_data", "Length=%d", evt->data_len);
            printf("%.*s\n", evt->data_len, (char *)evt->data);
            break;
        default:
            break;
    }

    return ESP_OK;
}

void fetch_data(){
    esp_http_client_config_t esp_http_client_config = {
        .url = "https://nuopbiwoomjqqfgdasxh.supabase.co/rest/v1/Teste?select=*",
        .method = HTTP_METHOD_GET,
        .event_handler = on_client_data,
        .buffer_size = 10000,              /*!< HTTP receive buffer size */
        // .buffer_size_tx = 10000           /*!< HTTP transmit buffer size */
    };

    esp_http_client_handle_t client = esp_http_client_init(&esp_http_client_config);
    esp_http_client_set_header(client, "apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im51b3BiaXdvb21qcXFmZ2Rhc3hoIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM3ODE5NzYsImV4cCI6MTk2OTM1Nzk3Nn0.OhT45KrI62zmA8TVxabm1dfeuyZhLD2O7tPp6NMXD2s");
    esp_http_client_set_header(client, "Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im51b3BiaXdvb21qcXFmZ2Rhc3hoIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM3ODE5NzYsImV4cCI6MTk2OTM1Nzk3Nn0.OhT45KrI62zmA8TVxabm1dfeuyZhLD2O7tPp6NMXD2s");
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK){
        ESP_LOGI("HTTP_Client", "HTTP GET status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
    }else{
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    // wifi_disconnect();
}

char *create_sacdm_body()
{
    // cJSON *json_payload = cJSON_CreateObject();
    cJSON *value = cJSON_CreateObject();
    // cJSON_AddItemToObject(json_payload, "value", value);

    cJSON_AddStringToObject(value, "value", "Hello do esp-idf no post bolado!");
    char *payload_body = cJSON_Print(value);
    
    return payload_body;
}

void send_data(){
    esp_http_client_config_t esp_http_client_config = {
        .url = "https://nuopbiwoomjqqfgdasxh.supabase.co/rest/v1/Teste?select=*", // Mudar URL
        .method = HTTP_METHOD_POST,
        .event_handler = on_client_data,
        .buffer_size = 10000,              /*!< HTTP receive buffer size */
        // .buffer_size_tx = 10000           /*!< HTTP transmit buffer size */
    };

    esp_http_client_handle_t client = esp_http_client_init(&esp_http_client_config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Prefer", "return=representation");
    esp_http_client_set_header(client, "apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im51b3BiaXdvb21qcXFmZ2Rhc3hoIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM3ODE5NzYsImV4cCI6MTk2OTM1Nzk3Nn0.OhT45KrI62zmA8TVxabm1dfeuyZhLD2O7tPp6NMXD2s");
    esp_http_client_set_header(client, "Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im51b3BiaXdvb21qcXFmZ2Rhc3hoIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM3ODE5NzYsImV4cCI6MTk2OTM1Nzk3Nn0.OhT45KrI62zmA8TVxabm1dfeuyZhLD2O7tPp6NMXD2s");

    char *payload_body = create_sacdm_body();
    esp_http_client_set_post_field(client, payload_body, strlen(payload_body));
    
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK){
        ESP_LOGI("HTTP_Client", "HTTP POST status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
    }else{
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
}

void app_main(void)
{
    ESP_LOGI("main", "Inicializando HTTP Client");
    ESP_ERROR_CHECK( nvs_flash_init() );
    wifi_init();
    ESP_ERROR_CHECK(wifi_connect_sta("ESPTest", "esp32_123", 10000));
    
    fetch_data();
    send_data();

    wifi_disconnect();
}
