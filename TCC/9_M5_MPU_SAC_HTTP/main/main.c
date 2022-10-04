#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <mpu6886.h>
#include "connect.h"
#include "cJSON.h"

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"

#include "esp_http_client.h"

float aX = 0.0, aY = 0.0, aZ = 0.0;

// SAC-DM Variable declaration
long lastMsg = 0, loopTimer = 0;
int value = 0, readings = 0, peaks_x = 0, peaks_y = 0, peaks_z = 0;
// float signals_x[3] = {0, 0, 0};
// float signals_y[3] = {0, 0, 0};
// float signals_z[3] = {0, 0, 0};
double rho_x, rho_y, rho_z;

int16_t signals_x[3] = {0, 0, 0};
int16_t signals_y[3] = {0, 0, 0};
int16_t signals_z[3] = {0, 0, 0};

int16_t accX = 0;
int16_t accY = 0;
int16_t accZ = 0;

i2c_port_t port = I2C_NUM_0;

#define SAMPLE_SIZE 500

float threshold = 1.20;

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
    cJSON *value = cJSON_CreateObject();
    char sac_values[100] = "";
    sprintf(sac_values, "rX: %f, rY: %f, rZ: %f", rho_x, rho_y, rho_z);
    cJSON_AddStringToObject(value, "value", sac_values);
    char *payload_body = cJSON_Print(value);
    
    return payload_body;
}

esp_http_client_config_t esp_http_client_config = {
    .url = "https://nuopbiwoomjqqfgdasxh.supabase.co/rest/v1/Teste2?",
    .method = HTTP_METHOD_POST,
    .event_handler = on_client_data,
    // .buffer_size = 10000,              /*!< HTTP receive buffer size */
    // .buffer_size_tx = 10000           /*!< HTTP transmit buffer size */
};

void send_data(esp_http_client_handle_t* client){
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
    // esp_http_client_cleanup(client);
}

void app_main(void)
{
    ESP_LOGI("main", "Inicializando HTTP Client");
    ESP_ERROR_CHECK( nvs_flash_init() );
    wifi_init();
    ESP_ERROR_CHECK(wifi_connect_sta("ESPTest", "esp32_123", 10000));

    mpu6886_init(&port);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    esp_http_client_handle_t client = esp_http_client_init(&esp_http_client_config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_header(client, "Prefer", "return=representation");
    esp_http_client_set_header(client, "apikey", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im51b3BiaXdvb21qcXFmZ2Rhc3hoIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM3ODE5NzYsImV4cCI6MTk2OTM1Nzk3Nn0.OhT45KrI62zmA8TVxabm1dfeuyZhLD2O7tPp6NMXD2s");
    esp_http_client_set_header(client, "Authorization", "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Im51b3BiaXdvb21qcXFmZ2Rhc3hoIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NTM3ODE5NzYsImV4cCI6MTk2OTM1Nzk3Nn0.OhT45KrI62zmA8TVxabm1dfeuyZhLD2O7tPp6NMXD2s");

    while(1){
        esp_err_t err = mpu6886_adc_accel_get( &accX, &accY, &accZ );

        if (err == ESP_OK){
            // ESP_LOGI("acc_values", "aX: %d, aY: %d, aZ: %d", accX, accY, accZ);
        }else{
            ESP_LOGE("acc_values", "Couldn't get values");
        }

        signals_x[0] = signals_x[1];
        signals_x[1] = signals_x[2];
        signals_x[2] = accX;

        signals_y[0] = signals_y[1];
        signals_y[1] = signals_y[2];
        signals_y[2] = accY;

        signals_z[0] = signals_z[1];
        signals_z[1] = signals_z[2];
        signals_z[2] = accZ;

        readings++;

        if (readings > 2) {
            if ((float)signals_x[1] > (float)signals_x[0]*threshold && (float)signals_x[1] > (float)signals_x[2]*threshold) peaks_x++;
            if ((float)signals_y[1] > (float)signals_y[0]*threshold && (float)signals_y[1] > (float)signals_y[2]*threshold) peaks_y++;
            if ((float)signals_z[1] > (float)signals_z[0]*threshold && (float)signals_z[1] > (float)signals_z[2]*threshold) peaks_z++;

        }
        if (readings == SAMPLE_SIZE) {
            rho_x = (float)peaks_x / (float)SAMPLE_SIZE;
            rho_y = (float)peaks_y / (float)SAMPLE_SIZE;
            rho_z = (float)peaks_z / (float)SAMPLE_SIZE;
            // ESP_LOGI("SAC_DM", "rho_x: %f, rho_y: %f, rho_z: %f", rho_x, rho_y, rho_z);
            send_data(client);
            readings = 1;
            peaks_x = 0;
            peaks_y = 0;
            peaks_z = 0;
        }
        vTaskDelay(8 / portTICK_PERIOD_MS);
    }

    wifi_disconnect();
}
