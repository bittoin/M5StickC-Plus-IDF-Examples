#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

void timer_task(TimerHandle_t xTimer) {
    static int count = 0;
    ESP_LOGI("Timer task", "Chamado %d vezes", count);
    count++;
}

void app_main(void)
{
    ESP_LOGI("main", "Programa iniciado!!");
    TimerHandle_t xTimer = xTimerCreate("Timer", (1000/portTICK_PERIOD_MS), pdTRUE, NULL, timer_task);
    xTimerStart(xTimer, 0);
}
