/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define M5_LED_GPIO (10)
#define DELAY_MS 500

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

void toggleLed(void *parameter){
    while(true) {
        /* Blink off (output low) */
        printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_reset_pin(M5_LED_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(M5_LED_GPIO, GPIO_MODE_OUTPUT);

    xTaskCreatePinnedToCore(
        toggleLed,      // Function that will be called when task starts
        "Toggle LED",   // Name of the task. Facilitate debugging.
        1024,           // Stack size in bytes
        NULL,           // Pointer to parameter
        1,              // Task priority
        NULL,           // Pointer to task handler, if it exists
        app_cpu         // CoreID of the board (0 or 1)
    );
}