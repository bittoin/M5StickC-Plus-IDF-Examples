// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"

// void task(void *params) {
//     int i = 0;
//     while (1) {
//         ESP_LOGI("Main", "[%d] Hello World!", i++);
//         vTaskDelay(500 / portTICK_PERIOD_MS);
//     }
// }

// void app_main(void)
// {
//     xTaskCreate(task, "Main Task", 2048, NULL, 1, NULL);
// }

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "I2Cbus.hpp"
#include "MPU.hpp"
#include "mpu/math.hpp"
#include "mpu/types.hpp"

static const char* TAG = "example";

#define SAMPLE_SIZE 500

float threshold = 1.20;

static constexpr gpio_num_t SDA = GPIO_NUM_21;
static constexpr gpio_num_t SCL = GPIO_NUM_22;
static constexpr uint32_t CLOCK_SPEED = 400000;  // range from 100 KHz ~ 400Hz

extern "C" void app_main() {
    printf("$ MPU Driver Example: MPU-I2C\n");
    fflush(stdout);

    // Initialize I2C on port 0 using I2Cbus interface
    i2c0.begin(SDA, SCL, CLOCK_SPEED);

    MPU_t MPU;  // create a default MPU object
    MPU.setBus(i2c0);  // set bus port, not really needed since default is i2c0
    MPU.setAddr(mpud::MPU_I2CADDRESS_AD0_LOW);  // set address, default is AD0_LOW

    // Great! Let's verify the communication
    // (this also check if the connected MPU supports the implementation of chip selected in the component menu)
    while (esp_err_t err = MPU.testConnection()) {
        ESP_LOGE(TAG, "Failed to connect to the MPU, error=%#X", err);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "MPU connection successful!");

    // Initialize
    ESP_ERROR_CHECK(MPU.initialize());  // initialize the chip and set initial configurations
    // Setup with your configurations
    ESP_ERROR_CHECK(MPU.setSampleRate(1000));  // set sample rate to 50 Hz
    // ESP_ERROR_CHECK(MPU.setGyroFullScale(mpud::GYRO_FS_500DPS));
    // ESP_ERROR_CHECK(MPU.setAccelFullScale(mpud::ACCEL_FS_4G));

    // Reading sensor data
    printf("Reading sensor data:\n");
    mpud::raw_axes_t accelRaw;   // x, y, z axes as int16
    mpud::raw_axes_t gyroRaw;    // x, y, z axes as int16
    mpud::float_axes_t accelG;   // accel axes in (g) gravity format
    mpud::float_axes_t gyroDPS;  // gyro axes in (DPS) º/s format

    // SAC-DM Variable declaration
    long lastMsg = 0, loopTimer = 0;
    int value = 0, readings = 0, peaks_x = 0, peaks_y = 0, peaks_z = 0;
    float signals_x[3] = {0, 0, 0};
    float signals_y[3] = {0, 0, 0};
    float signals_z[3] = {0, 0, 0};
    double rho_x, rho_y, rho_z;
    int count = 0;

    while (true) {
        // Read
        MPU.acceleration(&accelRaw);  // fetch raw data from the registers
        // MPU.rotation(&gyroRaw);       // fetch raw data from the registers
        // MPU.motion(&accelRaw, &gyroRaw);  // read both in one shot
        // Convert
        accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_4G);
        // gyroDPS = mpud::gyroDegPerSec(gyroRaw, mpud::GYRO_FS_500DPS);
        // Debug
        // printf("accel: [%+6.2f %+6.2f %+6.2f ] (G) \t\n", accelG.x, accelG.y, accelG.z);
        // printf("aX:%+6.2f,aY:%+6.2f,aZ:%+6.2f -- aX:%d,aY:%d,aZ:%d\n", accelG.x, accelG.y, accelG.z, accelRaw.x, accelRaw.y, accelRaw.z);
        // printf("gyro: [%+7.2f %+7.2f %+7.2f ] (º/s)\n", gyroDPS[0], gyroDPS[1], gyroDPS[2]);
        // vTaskDelay(2 / portTICK_PERIOD_MS);

        signals_x[0] = signals_x[1];
        signals_x[1] = signals_x[2];
        signals_x[2] = accelG.x;

        signals_y[0] = signals_y[1];
        signals_y[1] = signals_y[2];
        signals_y[2] = accelG.y;

        signals_z[0] = signals_z[1];
        signals_z[1] = signals_z[2];
        signals_z[2] = accelG.z;
        
        readings++;

        if (readings > 2) {
            if (signals_x[1] > signals_x[0]*threshold && signals_x[1] > signals_x[2]*threshold) peaks_x++;
            if (signals_y[1] > signals_y[0]*threshold && signals_y[1] > signals_y[2]*threshold) peaks_y++;
            if (signals_z[1] > signals_z[0]*threshold && signals_z[1] > signals_z[2]*threshold) peaks_z++;
            count++;
        }
        if (readings == SAMPLE_SIZE) {
            rho_x = (float)peaks_x / (float)SAMPLE_SIZE;
            rho_y = (float)peaks_y / (float)SAMPLE_SIZE;
            rho_z = (float)peaks_z / (float)SAMPLE_SIZE;
            // printf("MPU6886: rho_x: %f, rho_y: %f, rho_z: %f\n", rho_x, rho_y, rho_z);
            ESP_LOGI("SAC_DM", "count:%d, rho_x: %f, rho_y: %f, rho_z: %f", count, rho_x, rho_y, rho_z);
            readings = 1;
            peaks_x = 0;
            peaks_y = 0;
            peaks_z = 0;
            count = 0;
        }
        vTaskDelay(8 / portTICK_PERIOD_MS);
    }
}