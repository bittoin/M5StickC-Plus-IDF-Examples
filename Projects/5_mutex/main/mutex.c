/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

static SemaphoreHandle_t mutex;

// GLobal variable
static int shared_var = 0;

void incrementGlobalVariableTask(void *parameter){
    int local_var = 0;
    while(true) {
        local_var = shared_var;
        local_var++;
        int rand_number = (rand() % 400) + 100;
        shared_var = local_var;
        vTaskDelay(rand_number / portTICK_PERIOD_MS);
        printf("%d\n", shared_var);
    }
}

void app_main(void)
{
    // Creating mutex before start tasls
    mutex = xSemaphoreCreateMutex();

    /* Intializes random number generator */
    time_t t;
    srand((unsigned) time(&t));

    xTaskCreatePinnedToCore(
        incrementGlobalVariableTask,      // Function that will be called when task starts
        "Increment Task 1",   // Name of the task. Facilitate debugging.
        2048,           // Stack size in bytes
        NULL,           // Pointer to parameter
        1,              // Task priority
        NULL,           // Pointer to task handler, if it exists
        app_cpu         // CoreID of the board (0 or 1)
    );

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(
        incrementGlobalVariableTask,      // Function that will be called when task starts
        "Increment Task 2",   // Name of the task. Facilitate debugging.
        2048,           // Stack size in bytes
        NULL,           // Pointer to parameter
        1,              // Task priority
        NULL,           // Pointer to task handler, if it exists
        app_cpu         // CoreID of the board (0 or 1)
    );
}