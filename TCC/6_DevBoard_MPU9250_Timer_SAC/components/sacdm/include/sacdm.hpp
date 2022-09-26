#pragma once

#ifdef _cplusplus
extern "C"
{
#endif

#include "driver/gpio.h"
#include "driver/i2c.h"

#include "esp_err.h"
#include "esp_log.h"

// #define SAMPLE_SIZE 500

const int SAMPLE_SIZE = 500;
const float threshold = 1.20;

static constexpr gpio_num_t SDA = GPIO_NUM_21;
static constexpr gpio_num_t SCL = GPIO_NUM_22;
static constexpr uint32_t CLOCK_SPEED = 400000;  // range from 100 KHz ~ 400Hz

void init_sacdm();

#ifdef _cplusplus
}
#endif