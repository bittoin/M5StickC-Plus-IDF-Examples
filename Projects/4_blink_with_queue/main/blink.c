/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "driver/uart.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
// LEFT SERIAL COMMANDS WITH JUST 1 TASK
#define BLINK_GPIO 10
#define INPUT_BUFFER_SIZE 128
#define APP_CPU 1

// Queue
static QueueHandle_t queue_1;
static QueueHandle_t queue_2;
static const int queue_1_len = 5;   // Size of queue_1
static const int queue_2_len = 5;     // Size of queue_2

//uint16_t blinkPeriod = 1000;

void taskB(void *parameter){
    uint16_t ledDelay = 50;
    uint8_t counter = 0;
    const char maxCounterMessage[INPUT_BUFFER_SIZE] = "Blinked";
    while(1) {
        // See if there's a message in the queue (do not block)
        if (xQueueReceive(queue_1, (void *)&ledDelay, 0) == pdTRUE) {
            //printf("Novo delay: %d", incomingDelay);
            //printf("TaskB > From queue 1: %d\n", ledDelay);
        }
        // Blink led
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(ledDelay / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(ledDelay / portTICK_PERIOD_MS);
        counter++;

        if (counter >= 100){
            xQueueSend(queue_2, (void *)&maxCounterMessage, 10);
            counter = 1;
        }
    }
}

void taskA(void *parameter){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

    // Variables
    char input[INPUT_BUFFER_SIZE] = "";
    char temp[INPUT_BUFFER_SIZE] = "";
    char incoming_message[INPUT_BUFFER_SIZE] = "";
    int length = 0;
    
    char number[9] = "";
    
    while(1){
        // See if there's a message in the queue (do not block)
        if (xQueueReceive(queue_2, (void *)&incoming_message, 0) == pdTRUE) {
            printf("TaskA > From queue 2: %s\n", incoming_message);
        }
        // Read input from user and check if there is something on buffer to read
        ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM_0, (size_t*)&length));
        length = uart_read_bytes(UART_NUM_0, input, length, 100);
        if (length > 0){ // Length is > 0 when there is something on buffer
            for (int i = 0; i < length; ++i){
                if (input[i] == '\r'){ // Gets insert from keyboard
                    uint16_t newPeriod = atoi(number);
                    if (newPeriod != 0){
                        //blinkPeriod = newPeriod;
                        // Send integer to other task via queue
                        if (xQueueSend(queue_1, (void *)&newPeriod, 10) != pdTRUE) {
                            printf("TaskA > ERROR: Could not put item on delay queue.\n");
                        }
                        memset(number, 0, sizeof(number));
                    }
                    printf("TaskA > %s\r\n", temp);
                    memset(temp, 0, sizeof(temp));
                }
                strncat(temp, &input[i], 1); // https://www.geeksforgeeks.org/how-to-append-a-character-to-a-string-in-c/

                if (strstr(temp, "delay ") != NULL){ // https://www.delftstack.com/howto/c/string-contains-in-c/
                    strncat(number, &input[i], 1);  
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

    // Create queues
    queue_1 = xQueueCreate(queue_1_len, sizeof(uint16_t));
    queue_2 = xQueueCreate(queue_2_len, INPUT_BUFFER_SIZE*sizeof(char));

    xTaskCreatePinnedToCore(
        taskA,
        "Task A",
        4096,
        NULL,
        1,
        NULL,
        APP_CPU
    );

    xTaskCreatePinnedToCore(
        taskB,
        "Task B",
        1024,
        NULL,
        1,
        NULL,
        APP_CPU
    );
}