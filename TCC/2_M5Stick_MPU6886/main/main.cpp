#include <stdio.h>
#include <mpu6886.h>
#include <freertos/FreeRTOS.h>

float aX, aY, aZ;
i2c_port_t port = I2C_NUM_0;

extern "C" void app_main(void)
{
    aX = 0.0;
    aY = 0.0;
    aZ = 0.0;

    mpu6886_init(&port);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    //printf( "MPU6886 main: Init MPU6886 finished\n\n" );
    while(1){
        mpu6886_accel_data_get(&aX, &aY, &aZ);
        printf("ax:%f,aY:%f,aZ:%f\n", aX, aY, aZ);
        // printf("MPU6886: aX: %f, aY: %f, aZ: %f\n", aX, aY, aZ);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
