#include "IMU.hpp"

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <zephyr.h>
#include <cstring>
#include <stdint.h>

#include <memory>

namespace IMU
{
    std::unique_ptr<const device> hw;
    std::function<void()> cb;

    sensor_value temperature;
    sensor_value accel[3];
    sensor_value gyro[3];
    imu_data data;

    void handle_imu_data(const struct device *dev, struct sensor_trigger *trig);

    imu_packed& imu_packed::operator= (sensor_value &v)
    {
        value = (v.val2 & 0xFFFFFF) << 8;
        value |= (v.val1 & 0xFF);

        return *this;
    }

    void imu_packed::print(void)
    {
        int8_t whole = value;
        int32_t fraction = value >> 8;

        // Create the actual output result
        float res = (float)whole + (float)(fraction)/1000000.0f;
        printf("%g\n", res);
    }

    int init(const char *const label, std::function<void()> callback)
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

        cb = callback;

        return 0;
    }

    imu_data* raw()
    {
        return &data;
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

            data.timestamp = k_uptime_get_32();
            for (int i = 0; i < 3; i++) {
                data.accel[i] = accel[i];
                data.gyro[i] = gyro[i];
            }

            /* printf("[%s]:%g Cel\n" */
            /*     "  accel %f %f %f m/s/s\n" */
            /*     "  gyro  %f %f %f rad/s\n", */
            /*     "12:00", sensor_value_to_double(&temperature), */
            /*     sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]), */
            /*     sensor_value_to_double(&accel[2]), sensor_value_to_double(&gyro[0]), */
            /*     sensor_value_to_double(&gyro[1]), sensor_value_to_double(&gyro[2])); */

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

  /*
  const char *const label = DT_LABEL(DT_INST(0, invensense_icm42688));
  const struct device *icm42688 = device_get_binding(label);
  const struct device *led = device_get_binding(LED0);

  if (led == NULL) {
    printk("Failed to initialize diagnostic LED");
    return;
  }

  int ret = gpio_pin_configure(led, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
  if (ret < 0) {
    printk("Could not configure LED");
    return;
  }

  if (!icm42688) {
    printf("Failed to find sensor %s\n", "ICM42688");
    return;
  }

  tap_trigger = (struct sensor_trigger){
      .type = SENSOR_TRIG_TAP,
      .chan = SENSOR_CHAN_ALL,
  };

  if (sensor_trigger_set(icm42688, &tap_trigger, handle_icm42688_tap) < 0) {
    printf("Cannot configure tap trigger!!!\n");
    return;
  }

  double_tap_trigger = (struct sensor_trigger){
      .type = SENSOR_TRIG_DOUBLE_TAP,
      .chan = SENSOR_CHAN_ALL,
  };

  if (sensor_trigger_set(icm42688, &double_tap_trigger,
                         handle_icm42688_double_tap) < 0) {
    printf("Cannot configure double tap trigger!!!\n");
    return;
  }

  data_trigger = (struct sensor_trigger){
      .type = SENSOR_TRIG_DATA_READY,
      .chan = SENSOR_CHAN_ALL,
  };

  if (sensor_trigger_set(icm42688, &data_trigger, handle_icm42688_drdy) < 0) {
    printf("Cannot configure data trigger!!!\n");
    return;
  }

  printf("Configured for triggered sampling.\n");

  */
