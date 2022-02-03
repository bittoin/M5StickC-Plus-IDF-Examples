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
#define BLINK_GPIO 10
#define app_cpu 1

uint16_t blinkPeriod = 2000;
char *tmp;

void toggleLed(void *parameter){
    while(1) {
        /* Blink off (output low) */
        //printf("Turning off the LED\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(blinkPeriod / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        //printf("Turning on the LED\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(blinkPeriod / portTICK_PERIOD_MS);
    }
}

void readBlinkPeriod(void *parameter){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
    char data[128];
    int length = 0;
    char temp[128] = "";
    char number[9] = "";
    
    while(1){
        
        ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM_0, (size_t*)&length));
        length = uart_read_bytes(UART_NUM_0, data, length, 100);
        if (length > 0){
            for (int i = 0; i < length; ++i){
                if (data[i] == '\r'){ // Quando pega o ENTER do teclado
                    int newPeriod = atoi(number);
                    if (newPeriod != 0){
                        blinkPeriod = newPeriod;
                        memset(number, 0, sizeof(number));
                    }
                    printf("%s\n", temp);
                    memset(temp, 0, sizeof(temp));
                }
                strncat(temp, &data[i], 1); // https://www.geeksforgeeks.org/how-to-append-a-character-to-a-string-in-c/

                if (strstr(temp, "delay ") != NULL){ // https://www.delftstack.com/howto/c/string-contains-in-c/
                    strncat(number, &data[i], 1);  
                } 
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);


    xTaskCreatePinnedToCore(
        readBlinkPeriod,
        "Toggle LED",
        4096,
        NULL,
        1,
        NULL,
        app_cpu
    );

    xTaskCreatePinnedToCore(
        toggleLed,
        "Toggle LED",
        1024,
        NULL,
        1,
        NULL,
        app_cpu
    );
}