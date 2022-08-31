#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void task1(void *params) {
    while (1) {
        printf("Reading sensor on task1()\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *params) {
    while (1) {
        printf("Reading sensor on task2()\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    xTaskCreate(&task1, "Sensor 1", 2048, NULL, 2, NULL);
    xTaskCreate(&task2, "Sensor 2", 2048, NULL, 2, NULL);
}
