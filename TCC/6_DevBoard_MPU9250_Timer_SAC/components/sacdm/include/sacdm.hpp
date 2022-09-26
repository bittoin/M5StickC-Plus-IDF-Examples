#pragma once

#ifdef _cplusplus
extern "C"
{
#endif

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/timers.h"

#include "I2Cbus.hpp"
#include "MPU.hpp"

#include "esp_err.h"
#include "esp_log.h"

class SAC_DM {
    public:
        // SAC-DM Variable declaration
        static long lastMsg, loopTimer;
        static int value, readings, peaks_x, peaks_y, peaks_z;
        static float signals_x[3];
        static float signals_y[3];
        static float signals_z[3];
        static double rho_x, rho_y, rho_z;
        static int count;

        static MPU_t MPU;  // create a default MPU object

        // Reading sensor data
        static mpud::raw_axes_t accelRaw;   // x, y, z axes as int16
        static mpud::float_axes_t accelG;   // accel axes in (g) gravity format


        static SAC_DM& GetInstance() {
            // Allocate with `new` in case Singleton is not trivially destructible.
            static SAC_DM* sac_dm = new SAC_DM();
            return *sac_dm;
        }
        SAC_DM();
        void initialize_sac_dm();
        static void calculate();

    private:
    //SAC_DM() = default;
    // Delete copy/move so extra instances can't be created/moved.
    SAC_DM(const SAC_DM&) = delete;
    SAC_DM& operator=(const SAC_DM&) = delete;
    SAC_DM(SAC_DM&&) = delete;
    SAC_DM& operator=(SAC_DM&&) = delete;

    static constexpr const int SAMPLE_SIZE = 500;
    static constexpr const float threshold = 1.20;

    static constexpr gpio_num_t SDA = GPIO_NUM_21;
    static constexpr gpio_num_t SCL = GPIO_NUM_22;
    static constexpr uint32_t CLOCK_SPEED = 400000;  // range from 100 KHz ~ 400Hz
};

#ifdef _cplusplus
}
#endif