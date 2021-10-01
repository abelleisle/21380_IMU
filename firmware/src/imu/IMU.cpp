#include "IMU.hpp"

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <zephyr.h>
#include <cstring>

#include <memory>

namespace IMU
{
    std::unique_ptr<const device> hw;
    std::function<void()> cb;

    sensor_value temperature;
    sensor_value accel[3];
    sensor_value gyro[3];

    void handle_imu_data(const struct device *dev, struct sensor_trigger *trig);

    int init(const char *const label)
    {
        hw.reset(device_get_binding(label));

        if (!hw.get()) {
            printk("Failed to find sensor: %s\n", label);
            return 1;
        }

        static struct sensor_trigger data_trigger = {
            .type = SENSOR_TRIG_DATA_READY,
            .chan = SENSOR_CHAN_ALL
        };

        if (sensor_trigger_set(hw.get(), &data_trigger, handle_imu_data) < 0) {
            printk("Could not configure IMU trigger\n");
            return 1;
        }

        return 0;
    }

    int setCallback(std::function<void()> _cb)
    {
        cb = _cb;
    }

    int process_imu_data(const struct device *dev)
    {
        struct sensor_value tmp_temperature;
        struct sensor_value tmp_accel[3];
        struct sensor_value tmp_gyro[3];
        int err = sensor_sample_fetch(dev);

        if (!err)
            err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, tmp_accel);

        if (!err)
            err = sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, tmp_gyro);

        if (!err)
            err = sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP, &tmp_temperature);

        if (!err) {
            temperature = tmp_temperature;
            memcpy(accel, tmp_accel, 3*sizeof(sensor_value));
            memcpy(gyro, tmp_gyro, 3*sizeof(sensor_value));

            printf("[%s]:%g Cel\n"
                "  accel %f %f %f m/s/s\n"
                "  gyro  %f %f %f rad/s\n",
                "12:00", sensor_value_to_double(&temperature),
                sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]),
                sensor_value_to_double(&accel[2]), sensor_value_to_double(&gyro[0]),
                sensor_value_to_double(&gyro[1]), sensor_value_to_double(&gyro[2]));

            // TODO: error handle this
            cb();
        } else {
            printk("Sensor fetch failed.. Error: %d\n", err);
        }

        return err;
    }

    void handle_imu_data(const struct device *dev, struct sensor_trigger *trig)
    {
        int err = process_imu_data(dev);

        if (err) {
            printk("Trigger failed, disabling. Reason: %d\n", err);
            sensor_trigger_set(dev, trig, NULL);
            return;
        }
    }
}
