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

#include <logging/log.h>
LOG_MODULE_REGISTER(IMU_LOG, CONFIG_SENSOR_LOG_LEVEL);

//#define IMU_TRIGGER_EN

namespace IMU
{

/*******************************************************************************
*                                    MISC                                     *
*******************************************************************************/

    std::unique_ptr<const device> hw;
    std::function<void(imu_data*)> cb;
    bool hasCallback = false;

#if defined(IMU_TRIGGER_EN)
    void handle_imu_data(const struct device *dev, struct sensor_trigger *trig);
#else
    K_KERNEL_STACK_MEMBER(imu_thread_stack, 2048);
    struct k_thread imu_thread;
    void handle_imu_data(void);
    void blank_imu(const struct device *dev, struct sensor_trigger *trig){}
    void handle_virtual_data(void);
#endif

/*******************************************************************************
*                                DATA HANDLING                                *
*******************************************************************************/

    sensor_value temperature;
    sensor_value accel[3];
    sensor_value gyro[3];
    imu_data data;

    imu_packed& imu_packed::operator= (sensor_value &v)
    {
        data  = (v.val2 & 0xFFFFFF) << 8;
        data |= (v.val1 & 0xFF);

        return *this;
    }

    imu_packed& imu_packed::operator= (float v)
    {
        int8_t whole = (int8_t)v;
        int32_t frac = (v - (long)v) * 1000000.0f;

        data  = (frac & 0xFFFFFF) << 8;
        data |= (whole & 0xFF);

        return *this;
    }

    float imu_packed::value(void)
    {
        int8_t whole = data;
        int32_t fraction = data >> 8;

        // Create the actual output result
        float res = (float)whole + (float)(fraction)/1000000.0f;

        return res;
    }

    void imu_data::print(void)
    {
        uint32_t hour,min,sec,ms;
        ms = timestamp%1000;
        sec = (timestamp/1000)%60;
        min = (timestamp/60000)%60;
        hour = (timestamp/3600000);
        printf("%02" PRIu32 ":%02" PRIu32 ":%04" PRIu32 ",", min, sec, ms);
        printf("% 07.5f,% 07.5f,% 07.5f,% 07.5f,% 07.5f,% 07.5f \n",
                accel[0].value(), accel[1].value(), accel[2].value(),
                gyro[0].value(), gyro[1].value(), gyro[2].value());
    }

/*******************************************************************************
*                                  FUNCTIONS                                  *
*******************************************************************************/

    int init(const char *const label, std::function<void(imu_data*)> callback)
    {
        cb = callback;
        hasCallback = true;

        if (label == nullptr) {
            k_thread_create(&imu_thread, imu_thread_stack, 2048,
                (k_thread_entry_t)handle_virtual_data,
                NULL, NULL, NULL,
                K_PRIO_COOP(10),
                0, K_NO_WAIT);
            return 0;
        }

        hw.reset(device_get_binding(label));

        if (!hw.get()) {
            printk("Failed to find sensor: %s\n", label);
            return 1;
        }

        static struct sensor_trigger data_trigger = {
            .type = SENSOR_TRIG_DATA_READY,
            .chan = SENSOR_CHAN_ALL
        };

#if defined(IMU_TRIGGER_EN)
        if (sensor_trigger_set(hw.get(), &data_trigger, handle_imu_data) < 0) {
            printk("Could not configure IMU trigger\n");
            return 1;
        }
#else
        if (sensor_trigger_set(hw.get(), &data_trigger, blank_imu) < 0) {
            printk("Could not configure IMU trigger\n");
            return 1;
        }

        k_thread_create(&imu_thread, imu_thread_stack, 2048,
			(k_thread_entry_t)handle_imu_data,
            hw->data, NULL, NULL,
            K_PRIO_COOP(10),
			0, K_NO_WAIT);
#endif

        return 0;
    }

    imu_data* raw()
    {
        return &data;
    }

/*******************************************************************************
*                                  CALLBACKS                                  *
*******************************************************************************/

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

            // TODO: error handle this
            if (hasCallback)
                cb(&data);
        } else {
            printk("Sensor fetch failed.. Error: %d\n", err);
        }

        return err;
    }

#if defined(IMU_TRIGGER_EN)
    void handle_imu_data(const struct device *dev, struct sensor_trigger *trig)
    {
        int err = process_imu_data(dev);

        if (err) {
            printk("Trigger failed, disabling. Reason: %d\n", err);
            sensor_trigger_set(dev, trig, NULL);
            return;
        }
    }
#else
    void handle_imu_data(void)
    {
        while (1) {
            process_imu_data(hw.get());

            k_sleep(K_MSEC(40));
        }
    }
#endif
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
