#pragma once

#include <stdint.h>
#include <functional>
#include <drivers/sensor.h>

namespace IMU
{
    struct imu_packed {
        int8_t whole;
        int16_t fractional;

        void print(void);
        imu_packed& operator= (sensor_value &v);
    } __attribute__((packed));

    struct imu_data {
        union {
            struct {
                int16_t timestamp;
                imu_packed temperature;
                imu_packed accel[3];
                imu_packed gyro[3];
            };
            uint8_t raw[2 + sizeof(imu_packed)*7];
        };
    } __attribute__((packed));

    int init(const char* const label, std::function<void()> callback);
    imu_data* raw();
}
