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

/**
 * Enable hardware IMU trigger.
 * Uses GPIO trigger pins.
 *
 * Use this when there are either no GPIO trigger pins or the hardware triggers
 * aren't working.
 */
//#define IMU_TRIGGER_EN

namespace IMU
{

/*******************************************************************************
*                                    MISC                                     *
*******************************************************************************/

    std::unique_ptr<const device> hw;  /**< HW device pointer */
    std::function<void(imu_data*)> cb; /**< Callback executed whenever data is 
                                            obtained from IMU */
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

    sensor_value temperature; /**< Last temperature reading */
    sensor_value accel[3];    /**< Last accelerometer reading */
    sensor_value gyro[3];     /**< Last gyroscope reading */
    imu_data data;            /**< Packed IMU readings */

    /**
     * Packs a sensor_value into 32-bits
     *
     * @param[in] v sensor data to pack
     */
    imu_packed& imu_packed::operator= (sensor_value &v)
    {
        data  = (v.val2 & 0xFFFFFF) << 8;
        data |= (v.val1 & 0xFF);

        return *this;
    }

    /**
     * Sets the IMU data based on a float.
     *
     * @param[in] v float data to pack into 32-bits
     */
    imu_packed& imu_packed::operator= (float v)
    {
        int8_t whole = (int8_t)v;
        int32_t frac = (v - (long)v) * 1000000.0f;

        data  = (frac & 0xFFFFFF) << 8;
        data |= (whole & 0xFF);

        return *this;
    }

    /**
     * Get the float value of the imu data.
     *
     * @return The imu data interpolated as a float
     */
    float imu_packed::value(void)
    {
        int8_t whole = data;
        int32_t fraction = data >> 8;

        // Create the actual output result
        float res = (float)whole + (float)(fraction)/1000000.0f;

        return res;
    }

    /**
     * Print the current value of the IMU data.
     */
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

    /**
     * Initializes the IMU.
     *
     * @param[in] label the kernel's label for the IMU. If this is 'nullptr' a
     * virtual IMU will be created instead of a hardware IMU.
     * @param[in] callback the callback function that will be executed when IMU
     * data is recorded.
     *
     * @return Errors. 0 for success.
     */
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

    /**
     * Return the last measured IMU data.
     *
     * @warning doesn't guarantee that data has a value.
     *
     * @return The last piece of IMU data recorded.
     */
    imu_data* raw()
    {
        return &data;
    }

/*******************************************************************************
*                                  CALLBACKS                                  *
*******************************************************************************/

    /**
     * Reads all IMU data from hardware.
     * If any errors exist, stop and return.
     *
     * @param[in] dev the hardware IMU device being used.
     * @return The errors that occurred. 0 for success.
     */
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
    /**
     * Hardware trigger callback.
     *
     * @param[in] dev the hardware IMU that was triggered
     * @param[in] trig the IMU trigger settings
     */
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
    /**
     * Software trigger callback.
     * Run in a separate thread from the main processing thread.
     */
    void handle_imu_data(void)
    {
        while (1) {
            process_imu_data(hw.get());

            k_sleep(K_MSEC(40));
        }
    }
#endif
}
