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

// Bibliotecas do freeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Bibliotecas próprias para o projeto
#include "connect_sta.h"
#include "supabase_client.h"
#include "sacdm_manager.h"

const esp_timer_create_args_t esp_timer_create_args = {
        .callback = sacdm_calculate,
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

TaskHandle_t notify_handler = NULL;

void app_main(void) 
{
    // SAC_DM manager
    sacdm_init(&notify_handler);

    // Inicializa WiFi e conecta no AP
    ESP_LOGI("main", "Inicializando WiFi");
    esp_err_t ret = nvs_flash_init();        //Initialize NVS

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    wifi_init_sta();

    xTaskCreatePinnedToCore(&receive_http_notification, "SupabaseClient", 4096, NULL, 5, &notify_handler, 1);

    // Inicia timer para calculo do SAC-DM
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, 8*1000);
}
