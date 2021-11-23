#include "IMU.hpp"

#include "math.h"

namespace IMU
{

extern bool hasCallback;
extern std::function<void(imu_data*)> cb;

void handle_virtual_data(void)
{
    while (1) {
        uint32_t time = k_uptime_get_32();
        uint32_t offset = time % 4096;

        imu_data virt_data;

        virt_data.timestamp = time;

        float multiplier = (2 * SENSOR_PI)/(4.096f);

        virt_data.accel[0] = cosf(multiplier * offset);
        virt_data.accel[1] = sinf(multiplier * offset);
        virt_data.accel[2] = -9.81f; // Z

        virt_data.gyro[0] = 0.0f;
        virt_data.gyro[1] = 0.0f;
        virt_data.gyro[2] = 0.0f;

        if (hasCallback) {
            cb(&virt_data);
        }

        k_sleep(K_MSEC(40));
    }
}

} //namespace IMU
