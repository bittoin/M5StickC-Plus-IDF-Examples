/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "iot_button.h"

// create gpio button
button_config_t gpio_btn_cfg = {
    .type = BUTTON_TYPE_GPIO,
    .gpio_button_config = {
        .gpio_num = 37,
        .active_level = 0,
    },
};

const static char* TAG = "button_example";

static void button_single_click_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK");
}

static void button_double_click_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_DOUBLE_CLICK");
}

static void button_pressed_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_PRESSED");
}

static void button_released_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_RELEASED");
}

static void button_long_press_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_long_press");
}

static void button_long_press_hold_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_LONG_PRESS_HOLD");
}

void app_main(void)
{
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if(NULL == gpio_btn) {
        ESP_LOGE(TAG, "Button create failed");
    }
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_single_click_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_DOUBLE_CLICK, button_double_click_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_PRESS_DOWN, button_pressed_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_PRESS_UP, button_released_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, button_long_press_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_HOLD, button_long_press_hold_cb, NULL);
}
