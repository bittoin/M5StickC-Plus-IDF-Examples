#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <mpu6886.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"

#include "axp192.h"
#include "st7789.h"
#include "fontx.h"

#define INTERVAL 400
#define WAIT vTaskDelay(INTERVAL)

// M5stickC-Plus stuff
#define CONFIG_WIDTH 135
#define CONFIG_HEIGHT 240
#define CONFIG_MOSI_GPIO 15
#define CONFIG_SCLK_GPIO 13
#define CONFIG_CS_GPIO 5 
#define CONFIG_DC_GPIO 23
#define CONFIG_RESET_GPIO 18
#define CONFIG_BL_GPIO -1
#define CONFIG_OFFSETX 52
#define CONFIG_OFFSETY 40

static const char *TAG = "ST7789";

TFT_t dev;

static void SPIFFS_Directory(char * path) {
	DIR* dir = opendir(path);
	assert(dir != NULL);
	while (true) {
		struct dirent*pe = readdir(dir);
		if (!pe) break;
		ESP_LOGI(__FUNCTION__,"d_name=%s d_ino=%d d_type=%x", pe->d_name,pe->d_ino, pe->d_type);
	}
	closedir(dir);
}

float aX, aY, aZ;
i2c_port_t port = I2C_NUM_0;

#define SAMPLE_SIZE 500

float threshold = 1.20;

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

	esp_vfs_spiffs_conf_t conf = {
		.base_path = "/spiffs",
		.partition_label = NULL,
		.max_files = 10,
		.format_if_mount_failed =true
	};

	// Use settings defined above toinitialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is anall-in-one convenience function.
	esp_err_t ret =esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret == ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)",esp_err_to_name(ret));
		}
		return;
	}

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(NULL, &total,&used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG,"Failed to get SPIFFS partition information (%s)",esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG,"Partition size: total: %d, used: %d", total, used);
	}

    SPIFFS_Directory("/spiffs/");

    // power on
	i2c_master_init();
	AXP192_PowerOn();
	AXP192_ScreenBreath(11);


    FontxFile fx24G[2];
    InitFontx(fx24G,"/spiffs/ILGH24XB.FNT",""); // 12x24Dot Gothic

    spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO, CONFIG_BL_GPIO);
	lcdInit(&dev, CONFIG_WIDTH, CONFIG_HEIGHT, CONFIG_OFFSETX, CONFIG_OFFSETY);

#if CONFIG_INVERSION
	ESP_LOGI(TAG, "Enable Display Inversion");
	lcdInversionOn(&dev);
#endif

    char file[32];

    // Multi Font Test
    uint16_t color;
    uint8_t ascii[40];
    uint16_t margin = 10;
    lcdFillScreen(&dev, BLACK);
    color = WHITE;
    lcdSetFontDirection(&dev, 0);
    uint16_t xpos = 0;
    uint16_t ypos = 15;
    int xd = 0;
    int yd = 1;
    if(CONFIG_WIDTH < CONFIG_HEIGHT) {
        lcdSetFontDirection(&dev, 1);
        xpos = (CONFIG_WIDTH-1)-16;
        ypos = 0;
        xd = 1;
        yd = 0;
    }

    lcdDrawString(&dev, fx24G, 0, 15, (uint8_t*)"Iniciando programa!", WHITE);

    uint8_t text_X[40];
    uint8_t text_Y[40];
    uint8_t text_Z[40];

    aX = 0.0;
    aY = 0.0;
    aZ = 0.0;

    // SAC-DM Variable declaration
    long lastMsg = 0, loopTimer = 0;
    int value = 0, readings = 0, peaks_x = 0, peaks_y = 0, peaks_z = 0;
    double rho_x, rho_y, rho_z;

    int16_t signals_x[3] = {0, 0, 0};
    int16_t signals_y[3] = {0, 0, 0};
    int16_t signals_z[3] = {0, 0, 0};

    int16_t accX = 0;
    int16_t accY = 0;
    int16_t accZ = 0;

    mpu6886_init(&port);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    printf( "MPU6886 main: Init MPU6886 finished\n\n" );
    while(1){
        esp_err_t err = mpu6886_adc_accel_get( &accX, &accY, &accZ );

        if (err == ESP_OK){
            // ESP_LOGI("acc_values", "aX: %d, aY: %d, aZ: %d", accX, accY, accZ);
        }else{
            ESP_LOGE("acc_values", "Couldn't get values");
        }

        signals_x[0] = signals_x[1];
        signals_x[1] = signals_x[2];
        signals_x[2] = accX;

        signals_y[0] = signals_y[1];
        signals_y[1] = signals_y[2];
        signals_y[2] = accY;

        signals_z[0] = signals_z[1];
        signals_z[1] = signals_z[2];
        signals_z[2] = accZ;

        readings++;

        if (readings > 2) {
            if ((float)signals_x[1] > (float)signals_x[0]*threshold && (float)signals_x[1] > (float)signals_x[2]*threshold) peaks_x++;
            if ((float)signals_y[1] > (float)signals_y[0]*threshold && (float)signals_y[1] > (float)signals_y[2]*threshold) peaks_y++;
            if ((float)signals_z[1] > (float)signals_z[0]*threshold && (float)signals_z[1] > (float)signals_z[2]*threshold) peaks_z++;
        }
        if (readings == SAMPLE_SIZE) {
            rho_x = (float)peaks_x / (float)SAMPLE_SIZE;
            rho_y = (float)peaks_y / (float)SAMPLE_SIZE;
            rho_z = (float)peaks_z / (float)SAMPLE_SIZE;
            // printf("MPU6886: rho_x: %f, rho_y: %f, rho_z: %f\n", rho_x, rho_y, rho_z);
            // ESP_LOGI("SAC_DM", "rho_x: %f, rho_y: %f, rho_z: %f", rho_x, rho_y, rho_z);
            lcdFillScreen(&dev, BLACK);
            sprintf((char*)text_X, "rho_x: %f", rho_x);
            sprintf((char*)text_Y, "rho_y: %f", rho_y);
            sprintf((char*)text_Z, "rho_z: %f", rho_z);
            
            lcdDrawString(&dev, fx24G, 0, 15, text_X, WHITE);
            lcdDrawString(&dev, fx24G, 16, 15, text_Y, WHITE);
            lcdDrawString(&dev, fx24G, 32, 15, text_Z, WHITE);

            readings = 1;
            peaks_x = 0;
            peaks_y = 0;
            peaks_z = 0;
        }
        vTaskDelay(8 / portTICK_PERIOD_MS);
    }
}
