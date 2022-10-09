#include <stdio.h>
#include <string.h>

// Bibliotecas do ESP
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "sdkconfig.h"

// Tratamento de JSON
#include "cJSON.h"

// Bibliotecas do freeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Bibliotecas próprias para o projeto
#include "mpu6886.h"
#include "connect_sta.h"
#include "supabase_client.h"

/*
    ************************************
    Definição de variáveis para o SAC-DM
    ************************************
*/
#define SAMPLE_SIZE 500

float threshold = 1.20;
long lastMsg = 0, loopTimer = 0;
int value = 0, readings = 0, peaks_x = 0, peaks_y = 0, peaks_z = 0;
double rho_x, rho_y, rho_z;

int16_t signals_x[3] = {0, 0, 0};
int16_t signals_y[3] = {0, 0, 0};
int16_t signals_z[3] = {0, 0, 0};

int16_t accX = 0;
int16_t accY = 0;
int16_t accZ = 0;

i2c_port_t port = I2C_NUM_0;

char *create_sacdm_payload_body()
{
    cJSON *value = cJSON_CreateObject();
    char sac_values[100] = "";
    sprintf(sac_values, "rX:%f,rY:%f,rZ:%f", rho_x, rho_y, rho_z);
    cJSON_AddStringToObject(value, "value", sac_values);
    char *payload_body = cJSON_Print(value);
    
    return payload_body;
}

/*
    *************************************************
    Definição de variáveis e funções das notificações
    *************************************************
*/

static TaskHandle_t receiverHandler = NULL;

void send_sac_dm_notification(void *params) 
{
    // Inicializa MPU
    ESP_LOGI("main", "Inicializando MPU6886");
    mpu6886_init(&port);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    while(1){
        // Atualiza dados da MPU em cada variável de aceleração
        esp_err_t err = mpu6886_adc_accel_get( &accX, &accY, &accZ );

        // Testa se houve algum erro na coleta dos dados
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
            xTaskNotifyGive(receiverHandler);
            readings = 1;
            peaks_x = 0;
            peaks_y = 0;
            peaks_z = 0;
        }
        // vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void sac_dm_calculate(void *params)
{
    // Atualiza dados da MPU em cada variável de aceleração
    esp_err_t err = mpu6886_adc_accel_get( &accX, &accY, &accZ );

    // Testa se houve algum erro na coleta dos dados
    if (err == ESP_OK) {
        // ESP_LOGI("acc_values", "aX: %d, aY: %d, aZ: %d", accX, accY, accZ);
    }
    else {
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
        xTaskNotifyGive(receiverHandler);
        readings = 1;
        peaks_x = 0;
        peaks_y = 0;
        peaks_z = 0;
    }
}

const esp_timer_create_args_t esp_timer_create_args = {
        .callback = sac_dm_calculate,
        .name = "SAC Timer"
    };
esp_timer_handle_t esp_timer_handle;

void creating_timer()
{
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, 2*1000);
}

void receive_http_notification(void *params)
{
    // Configura e inicializa supabase
    const supabase_config sbp_config = {
        .table_url = "",
        .api_key = ""
    };
    spb_open(&sbp_config);

    while(true) {
        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        spb_write(create_sacdm_payload_body());
    }
}

void app_main(void) 
{
    // Inicializa I2C + MPU
    ESP_LOGI("main", "Inicializando MPU6886");
    mpu6886_init(&port);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Inicializa WiFi e conecta no AP
    ESP_LOGI("main", "Inicializando WiFi");
    esp_err_t ret = nvs_flash_init();        //Initialize NVS

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    wifi_init_sta();

    xTaskCreatePinnedToCore(&receive_http_notification, "SupabaseClient", 4096, NULL, 5, &receiverHandler, 1);

    // Inicia timer para calculo do SAC-DM
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, 8*1000);
}
