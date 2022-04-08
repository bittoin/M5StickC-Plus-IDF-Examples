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

#include <string.h>
#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "driver/uart.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define M5_LED_GPIO (10)

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

uint16_t blinkPeriod = 1000;

void toggleLed(void *parameter){
    gpio_reset_pin(M5_LED_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(M5_LED_GPIO, GPIO_MODE_OUTPUT);

    while(true) {
        /* Blink off (output low) */
        gpio_set_level(M5_LED_GPIO, 0);
        vTaskDelay(blinkPeriod / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(M5_LED_GPIO, 1);
        vTaskDelay(blinkPeriod / portTICK_PERIOD_MS);
    }
}

void consoleApp(void *parameter){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
    char chr[9];

    while(1){
        printf("Enter value (0 - 65536) in ms to blink: \n");
        gets(chr);
        uint16_t newPeriod = atoi(chr);
        if (newPeriod != 0){    // Valid number
            blinkPeriod = newPeriod;
            printf("Value: %d(ms)\n", blinkPeriod);
        }else{                  // Invalid number
            printf("Number not valid!\n");
        }
    }
}

void app_main(void){
    xTaskCreatePinnedToCore(
        consoleApp,
        "Console app",
        4096,
        NULL,
        1,
        NULL,
        app_cpu
    );

    xTaskCreatePinnedToCore(
        toggleLed,
        "Toggle LED",
        4096,
        NULL,
        1,
        NULL,
        app_cpu
    );
}