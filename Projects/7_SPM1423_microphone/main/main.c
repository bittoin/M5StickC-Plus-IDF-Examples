// #include <stdio.h>
// #include "driver/i2s.h"
// #include "esp_err.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "core2forAWS.h"

// // Definição para configuração do periférico
// #define LEDC_TIMER              LEDC_TIMER_0
// #define LEDC_MODE               LEDC_LOW_SPEED_MODE
// #define LEDC_OUTPUT_IO          (10) // Define the output GPIO
// #define LEDC_CHANNEL            LEDC_CHANNEL_0
// #define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
// #define LEDC_DUTY               (255) // Set duty cycle
// #define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz

// #if CONFIG_FREERTOS_UNICORE
//     static const BaseType_t app_cpu = 0;
// #else
//     static const BaseType_t app_cpu = 1;
// #endif

// const int M5_PIN_MIC_CLK = 0;         
// const int M5_PIN_MIC_DATA = 34; 

// void read_microphone(void *parameter){
//     i2s_driver_uninstall(I2S_NUM_0); 
//     i2s_config_t i2s_config = {        
//         .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
//         .sample_rate = 44100,       //.sample_rate = MIC_SAMPLING_RATE,       
//         .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,         
//         .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT, 
//         .communication_format = I2S_COMM_FORMAT_I2S,        
//         .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,        
//         .dma_buf_count = 8,        
//         .dma_buf_len = 1024,   
//     };

//     i2s_pin_config_t pin_config;
//     pin_config.bck_io_num = I2S_PIN_NO_CHANGE; 
//     pin_config.ws_io_num = M5_PIN_MIC_CLK; 
//     pin_config.data_out_num = I2S_PIN_NO_CHANGE; 
//     pin_config.data_in_num = M5_PIN_MIC_DATA;

//     ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL)); 
//     ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_0, &pin_config));

//     char *micBuffer[1024];
//     size_t bytesread;

//     while (1) {
//         i2s_read(I2S_NUM_0, (char *)micBuffer, 1024/*MIC_BUFFER_SIZE*/, &bytesread, (500 / portTICK_RATE_MS));

//         for (uint i = 0; i < bytesread; i += 2) {
//             int16_t *val = (int16_t *)&micBuffer[i]; 
//             printf("Valor microphone: %ls", val);
//         }
//     }
// }

// void app_main(void)
// {
//     gpio_reset_pin(LEDC_OUTPUT_IO);
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(LEDC_OUTPUT_IO, GPIO_MODE_OUTPUT);

//     xTaskCreatePinnedToCore(
//         read_microphone,      // Function that will be called when task starts
//         "sampleMicrophoneTask",   // Name of the task. Facilitate debugging.
//         4*2048,           // Stack size in bytes
//         NULL,           // Pointer to parameter
//         1,              // Task priority
//         NULL,           // Pointer to task handler, if it exists
//         app_cpu         // CoreID of the board (0 or 1)
//     );
// }

// -------------------------------------------------------------------------------------------------------------

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "core2forAWS.h"

// #define CONFIG_SOFTWARE_AUDIO_SUPPORT
 
// static const char *TAG = "EXAMPLE";
 
// static void microphone_task(void *arg){
//     /* If the speaker was initialized, be sure to call Speaker_Deinit() and 
//         disable first. */
//     Microphone_Init();
 
//     static int8_t i2s_readraw_buf[1024];
//     size_t bytes_read;
 
//     i2s_read(MIC_I2S_NUMBER, (char*)i2s_readraw_buf, 1024, &bytes_read, pdMS_TO_TICKS(100));
//     Microphone_Deinit();
 
//     for(uint16_t i = 0; i < 1024; i++){
//         ESP_LOGI(TAG, "At buffer index %d heard: %d", i, i2s_readraw_buf[i]);
//     }
//     ESP_LOGI(TAG, "Read total of %u bytes from microphone", bytes_read);
//     vTaskDelete(NULL);
// }
 
// void app_main(void){
//     Core2ForAWS_Init();
 
//     /* Usage of the microphone must be pinned to a specific core since I2S ISR 
//         is assigned per core */
//     xTaskCreatePinnedToCore(microphone_task, "listen", 4096*2, NULL, 4, NULL, 1);
// }

#include "core2foraws.h"
 
void app_main(void){
 
}