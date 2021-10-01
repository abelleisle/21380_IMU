/*
 * Copyright (c) 2020 TDK Invensense
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <zephyr.h>

#include <bt/BLE.hpp>
#include <imu/IMU.hpp>

void imu_callback(void)
{
    IMU::imu_data* imu = IMU::raw();
    /* printk("IMU Data: "); */
    /* imu->accel[2].print(); */
    /* printk("IMU Raw: %d\t%d\n", imu->accel[2].whole, imu->accel[2].fractional); */
    BLE::setAdData(imu->raw, sizeof(imu->raw));
}

void main(void) {
    BLE::init();
    IMU::init(DT_LABEL(DT_INST(0, invensense_icm42688)), imu_callback);
        

    /*
    while (0) {
        printk("LED On...\n");
        gpio_pin_set(led, PIN, false);
        k_msleep(500);
        printk("LED Off...\n");
        gpio_pin_set(led, PIN, true);
        k_msleep(500);
    }
    */


  return;
}
