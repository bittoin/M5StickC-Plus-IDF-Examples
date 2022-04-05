#include <stdio.h>
#include <string.h>
#include "driver/i2s.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// based on https://github.com/m5stack/M5StickC/blob/master/examples/Basics/Micophone/Micophone.ino

#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

#define M5_PIN_MIC_CLK (GPIO_NUM_0)
#define M5_PIN_MIC_DATA (GPIO_NUM_34)
#define I2S_NUM (0)
#define I2S_BCK_IO (GPIO_NUM_4)
#define I2S_WS_IO (GPIO_NUM_5)
#define I2S_MCK_IO (GPIO_NUM_0)
#define I2S_BCK_IO (GPIO_NUM_4)
#define I2S_DO_IO (GPIO_NUM_18)
#define I2S_DI_IO (GPIO_NUM_19)
#define RECV_BUF_SIZE (2*256)
#define GAIN_FACTOR 3

static const char *TAG = "I2S_SPM1423";
static const char err_reason[][30] = {"input param is invalid",
                                      "operation timeout"
                                     };

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static esp_err_t i2s_driver_init(void)
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,        
        .dma_buf_len = 128,   
    };

    i2s_pin_config_t i2s_pin_config = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_PIN_NO_CHANGE,
        .ws_io_num = M5_PIN_MIC_CLK,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = M5_PIN_MIC_DATA
    };

    ESP_RETURN_ON_ERROR(i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL), TAG, "install i2s failed");
    ESP_RETURN_ON_ERROR(i2s_set_pin(I2S_NUM, &i2s_pin_config), TAG, "set i2s pins failed");
    ESP_RETURN_ON_ERROR(i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO), TAG, "set clk i2s failed");
    return ESP_OK;
}

void read_microphone(void *parameter){
    uint8_t BUFFER[RECV_BUF_SIZE] = {0};

    uint16_t oldy[160];
    int16_t *adcBuffer = NULL;

    esp_err_t ret = ESP_OK;
    size_t bytesread = 0;

    while (1) {
        // Reading data
        ret = i2s_read(I2S_NUM, (char *)BUFFER, RECV_BUF_SIZE, &bytesread, (100 / portTICK_RATE_MS));

        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[echo] i2s read failed, %s", err_reason[ret == ESP_ERR_TIMEOUT]);
            abort();
        }

        adcBuffer = (int16_t *)BUFFER;

        // Showing data
        int y;
        y = adcBuffer[0] * GAIN_FACTOR;
        printf("%d\n", y);

        vTaskDelay( 50 / portTICK_RATE_MS);
    }
}

void app_main(void)
{   
    printf("Vai inicializar o i2s\n");
    /* Initialize i2s peripheral */
    if (i2s_driver_init() != ESP_OK) {
        ESP_LOGE(TAG, "i2s driver init failed");
        abort();
    }

    printf("Já inicializou o i2s\n");

    xTaskCreatePinnedToCore(
        read_microphone,      // Function that will be called when task starts
        "sampleMicrophoneTask",   // Name of the task. Facilitate debugging.
        4*2048,           // Stack size in bytes
        NULL,           // Pointer to parameter
        1,              // Task priority
        NULL,           // Pointer to task handler, if it exists
        app_cpu         // CoreID of the board (0 or 1)
    );
}

// -------------------------------------------------------------------------------------------------------------