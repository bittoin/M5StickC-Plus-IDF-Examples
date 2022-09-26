#include <stdio.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "sacdm.hpp"

extern "C" void app_main();

static const char* TAG = "MAIN";

void app_main() {
    ESP_LOGI(TAG, "$ MPU Driver Example: MPU-I2C\n");
    fflush(stdout);
    
    init_sacdm();
    // TimerHandle_t xTime = xTimerCreate("Timer sac-dm", (2/portTICK_PERIOD_MS), pdTRUE, NULL, measure_sac_dm);
    // xTimerStart(xTime, 0);
}