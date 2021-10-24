#pragma once

#include <stdint.h>
#include <functional>
#include <drivers/sensor.h>

namespace IMU
{
    struct imu_packed {
        int32_t data;

        float value(void);
        imu_packed& operator= (sensor_value &v);
    } __attribute__((packed));

    struct imu_data {
        union {
            struct {
                uint32_t timestamp;
                imu_packed accel[3];
                imu_packed gyro[3];
            };
            uint8_t raw[2 + sizeof(imu_packed)*7];
        };
        void print(void);
    } __attribute__((packed));

    int process_imu_data(const struct device *dev);
    int init(const char* const label, std::function<void(imu_data*)> callback);
    imu_data* raw();
}
