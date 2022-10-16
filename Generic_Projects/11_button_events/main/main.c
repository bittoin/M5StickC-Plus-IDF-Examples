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
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "iot_button.h"

#define M5_BUTTON_A 37
#define M5_BUTTON_B 39
#define M5_LED GPIO_NUM_10
#define M5_BUZZER GPIO_NUM_2

static int led_state = 1;

// create gpio button
button_config_t gpio_btn_cfg = {
    .type = BUTTON_TYPE_GPIO,
    .gpio_button_config = {
        .gpio_num = M5_BUTTON_A,
        .active_level = 0,
    },
};

const static char* TAG = "button_example";

static void button_single_click_cb(void *arg)
{
    ESP_LOGI(TAG, "BUTTON_SINGLE_CLICK");
    gpio_set_level(M5_LED, led_state);
    led_state = !led_state;
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
    // gpio_reset_pin(M5_LED);
    gpio_pad_select_gpio(M5_LED);
    gpio_set_direction(M5_LED, GPIO_MODE_OUTPUT);
    gpio_set_level(M5_LED, 0);
    
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if(NULL == gpio_btn) {
        ESP_LOGE(TAG, "Button create failed");
    }
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_single_click_cb, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_DOUBLE_CLICK, button_double_click_cb, NULL);
    // iot_button_register_cb(gpio_btn, BUTTON_PRESS_DOWN, button_pressed_cb, NULL);
    // iot_button_register_cb(gpio_btn, BUTTON_PRESS_UP, button_released_cb, NULL);
    // iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_START, button_long_press_cb, NULL);
    // iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_HOLD, button_long_press_hold_cb, NULL);
}
