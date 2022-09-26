#include "sacdm.hpp"

#include "mpu/math.hpp"
#include "mpu/types.hpp"

static const char* TAG = "SACDM";

// SAC-DM Variable declaration
long SAC_DM::lastMsg = 0, SAC_DM::loopTimer = 0;
int SAC_DM::value = 0, SAC_DM::readings = 0, SAC_DM::peaks_x = 0, SAC_DM::peaks_y = 0, SAC_DM::peaks_z = 0;
float SAC_DM::signals_x[3] = {0, 0, 0};
float SAC_DM::signals_y[3] = {0, 0, 0};
float SAC_DM::signals_z[3] = {0, 0, 0};
double SAC_DM::rho_x = 0, SAC_DM::rho_y = 0, SAC_DM::rho_z = 0;
int SAC_DM::count = 0;

mpud::raw_axes_t SAC_DM::accelRaw;
mpud::float_axes_t SAC_DM::accelG;

MPU_t SAC_DM::MPU;  // create a default MPU object

SAC_DM::SAC_DM(){
    // Initialize I2C on port 0 using I2Cbus interface
    i2c0.begin(SDA, SCL, CLOCK_SPEED);

    MPU.setBus(i2c0);  // set bus port, not really needed since default is i2c0
    MPU.setAddr(mpud::MPU_I2CADDRESS_AD0_LOW);  // set address, default is AD0_LOW
}

void SAC_DM::initialize_sac_dm()
{
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
}

void SAC_DM::calculate() {
    // Read
    MPU.acceleration(&accelRaw);  // fetch raw data from the registers
    
    accelG = mpud::accelGravity(accelRaw, mpud::ACCEL_FS_8G);

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
        // printf("rho_x:%f,rho_y:%f,rho_z:%f\n", rho_x, rho_y, rho_z);
        ESP_LOGI("SAC_DM", "count:%d, rho_x: %f, rho_y: %f, rho_z: %f", count, rho_x, rho_y, rho_z);
        readings = 1;
        peaks_x = 0;
        peaks_y = 0;
        peaks_z = 0;
        count = 0;
    }
}