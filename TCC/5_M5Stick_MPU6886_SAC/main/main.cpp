#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <mpu6886.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (10) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (253) // Set duty cycle
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz

float aX, aY, aZ;
i2c_port_t port = I2C_NUM_0;

#define SAMPLE_SIZE 500

float threshold = 1.20;

// static void example_ledc_init(void)
// {
//     // Prepare and then apply the LEDC PWM timer configuration
//     ledc_timer_config_t ledc_timer = {
//         .speed_mode       = LEDC_MODE,
//         .timer_num        = LEDC_TIMER,
//         .duty_resolution  = LEDC_DUTY_RES,
//         .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
//         .clk_cfg          = LEDC_AUTO_CLK
//     };
//     ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

//     // Prepare and then apply the LEDC PWM channel configuration
//     ledc_channel_config_t ledc_channel = {
//         .speed_mode     = LEDC_MODE,
//         .channel        = LEDC_CHANNEL,
//         .timer_sel      = LEDC_TIMER,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = LEDC_OUTPUT_IO,
//         .duty           = 0, // Set duty to 0%
//         .hpoint         = 0
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

//     // // Set duty to 5%
//     ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
//     // Update duty to apply the new value
//     ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
// }

extern "C" void app_main(void)
{
    // example_ledc_init();

    aX = 0.0;
    aY = 0.0;
    aZ = 0.0;

    // SAC-DM Variable declaration
    long lastMsg = 0, loopTimer = 0;
    int value = 0, readings = 0, peaks_x = 0, peaks_y = 0, peaks_z = 0;
    float signals_x[3] = {0, 0, 0};
    float signals_y[3] = {0, 0, 0};
    float signals_z[3] = {0, 0, 0};
    double rho_x, rho_y, rho_z;

    mpu6886_init(&port);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    //printf( "MPU6886 main: Init MPU6886 finished\n\n" );
    while(1){
        mpu6886_accel_data_get(&aX, &aY, &aZ);
        // printf("ax:%f,aY:%f,aZ:%f\n", aX, aY, aZ);
        // printf("MPU6886: aX: %f, aY: %f, aZ: %f\n", aX, aY, aZ);
        // vTaskDelay(50 / portTICK_PERIOD_MS);

        signals_x[0] = signals_x[1];
        signals_x[1] = signals_x[2];
        signals_x[2] = aX;

        signals_y[0] = signals_y[1];
        signals_y[1] = signals_y[2];
        signals_y[2] = aY;

        signals_z[0] = signals_z[1];
        signals_z[1] = signals_z[2];
        signals_z[2] = aZ;
        
        readings++;

        if (readings > 2) {
            if (signals_x[1] > signals_x[0]*threshold && signals_x[1] > signals_x[2]*threshold) peaks_x++;
            if (signals_y[1] > signals_y[0]*threshold && signals_y[1] > signals_y[2]*threshold) peaks_y++;
            if (signals_z[1] > signals_z[0]*threshold && signals_z[1] > signals_z[2]*threshold) peaks_z++;
        }
        if (readings == SAMPLE_SIZE) {
            rho_x = (float)peaks_x / (float)SAMPLE_SIZE;
            rho_y = (float)peaks_y / (float)SAMPLE_SIZE;
            rho_z = (float)peaks_z / (float)SAMPLE_SIZE;
            // printf("MPU6886: rho_x: %f, rho_y: %f, rho_z: %f\n", rho_x, rho_y, rho_z);
            ESP_LOGI("SAC_DM", "rho_x: %f, rho_y: %f, rho_z: %f", rho_x, rho_y, rho_z);
            readings = 1;
            peaks_x = 0;
            peaks_y = 0;
            peaks_z = 0;
        }
        vTaskDelay(8 / portTICK_PERIOD_MS);
    }
}
