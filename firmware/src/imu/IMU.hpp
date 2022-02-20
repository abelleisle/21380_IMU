#pragma once

#include <stdint.h>
#include <functional>
#include <drivers/sensor.h>

namespace IMU
{
    /**
     * Packed IMU data.
     *
     * Packs the 64-bit sensor data into 32-bits. 8 bits of integer and 24 bits
     * of decimal. Packed like so: 0xDDDDDDII
     * 
     * The decimal is the fraction of 1,000,000.
     */
    struct imu_packed {
        int32_t data; /**< The raw data. */

        float value(void);
        imu_packed& operator= (sensor_value &v);
        imu_packed& operator= (float f);
    } __attribute__((packed));

    /**
     * IMU data packet.
     * All 3 axis of accel, gyro, and timestamp.
     */
    struct imu_data {
        union {
            struct {
                uint32_t timestamp;  /**< System timestamp */
                imu_packed accel[3]; /**< X,Y,Z accelerometer data */
                imu_packed gyro[3];  /**< X,Y,Z gyroscope data */
            };
            uint8_t raw[4 + sizeof(imu_packed)*6]; /**< Raw imu data bytes */
        };
        void print(void);
    } __attribute__((packed));

    int process_imu_data(const struct device *dev);
    int init(const char* const label, std::function<void(imu_data*)> callback);
    imu_data* raw();
}
