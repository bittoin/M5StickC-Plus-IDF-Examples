#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

xSemaphoreHandle mutexCommonAccess = NULL;

void commonAccess(const char *str) {
    printf("%s", str);
}

void task1(void *params) {
    while (1) {
        printf("Entering task 1\n");
        // Se consegue pegar o semáforo livre
        if (xSemaphoreTake(mutexCommonAccess, 1000 / portTICK_PERIOD_MS)) {
            commonAccess("Reading sensor on task1(x)\n");
            xSemaphoreGive(mutexCommonAccess);
        }else{
            printf("task 1 read sensor timeout\n");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *params) {
    while (1) {
        printf("Entering task 2\n");
        if (xSemaphoreTake(mutexCommonAccess, 1000 / portTICK_PERIOD_MS)) {
            commonAccess("Reading sensor on task2(y)\n");
            xSemaphoreGive(mutexCommonAccess);
        }else{
            printf("task 2 read sensor timeout\n");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    mutexCommonAccess = xSemaphoreCreateMutex();
    xTaskCreate(&task1, "Task 1", 2048, NULL, 2, NULL);
    xTaskCreate(&task2, "Task 2", 2048, NULL, 2, NULL);
}
