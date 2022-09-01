#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static TaskHandle_t receiverHandler = NULL;

void send_notification(void *params) {
    while (true) {
        xTaskNotifyGive(receiverHandler);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void receive_notification(void *params) {
    while (true) {
        ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        ESP_LOGI("NotificationReceiver", "Notification received!");
    }
}

void app_main(void)
{
    xTaskCreate(&receive_notification, "NotificationReceiver", 2048, NULL, 5, &receiverHandler);
    xTaskCreate(&send_notification, "NotificationSender", 2048, NULL, 5, NULL);
}
