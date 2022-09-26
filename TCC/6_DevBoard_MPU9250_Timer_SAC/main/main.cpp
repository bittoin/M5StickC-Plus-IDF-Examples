#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_timer.h"

#include "sacdm.hpp"

extern "C" void app_main();

static const char* TAG = "MAIN";

void call_calculate_timer(void *args) {
    SAC_DM::calculate();
}

void app_main() {
    ESP_LOGI(TAG, "$ MPU Driver Example: MPU-I2C\n");
    fflush(stdout);
    SAC_DM sac;
    sac.initialize_sac_dm();
    // TimerHandle_t xTime = xTimerCreate("Timer sac-dm", ( 10 / portTICK_PERIOD_MS ), pdTRUE, NULL, call_calculate_timer);
    // xTimerStart(xTime, 0);
    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = call_calculate_timer,
        .name = "SAC Timer"
    };
    esp_timer_handle_t esp_timer_handle;
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, 2*1000);
}