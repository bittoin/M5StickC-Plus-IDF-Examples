#include <stdio.h>
#include <mpu6886.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define LED_GPIO (10)

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

i2c_port_t port = I2C_NUM_0;

void showMpuAccValues(void *parameter){
    float aX = 0.0;
    float aY = 0.0;
    float aZ = 0.0;

    printf( "MPU6886 main: Init MPU6886 finished\n\n" );
    mpu6886_init(&port);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    printf( "MPU6886 main: finished MPU6886 config\n\n" );
    while(1){
        mpu6886_accel_data_get(&aX, &aY, &aZ);
        printf("MPU6886: aX: %f, aY: %f, aZ: %f\n", aX, aY, aZ);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(
        showMpuAccValues,
        "MPU Acc Data",
        4096,
        NULL,
        1,
        NULL,
        app_cpu
    );
}
