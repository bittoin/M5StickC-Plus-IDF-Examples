#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "esp_system.h"

#define TAG "RANDOM_NUMBER"

void app_main(void)
{
    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        int D20 = (esp_random() % 20) + 1;
        ESP_LOGI(TAG, "Random 20 dice: %d", D20);
    }
}
