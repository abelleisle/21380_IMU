/*
 * Copyright (c) 2020 TDK Invensense
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <zephyr.h>
#include <zephyr/types.h>
#include <kernel.h>

#include <bt/BLE.hpp>
#include <imu/IMU.hpp>
#include <term/term.hpp>
#include <sys/msg.hpp>

#include <stdio.h>
#include <cstring>
#include <queue>

const unsigned int max_readings = 512;
std::queue<IMU::imu_data> imuData;
std::queue<IMU::imu_data> tmpData; /**< This is used while BLE transfer is occurring */

bool running = false;
bool streaming = false;

/*******************************************************************************
*                                BT CALLBACKS                                 *
*******************************************************************************/

ssize_t imu_data_send(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                    void *buf, uint16_t len, uint8_t offset)
{
    if (len == 4) {
    // Send the size of the queue
        uint32_t size = imuData.size();
        return bt_gatt_attr_read(conn, attr, buf, len, offset, (void*)&size, sizeof(size));
    } else if (len == sizeof(IMU::imu_data)) {
    // Send the the front IMU data
        if (imuData.size()) {
            IMU::imu_data data = imuData.front();
            imuData.pop();
            return bt_gatt_attr_read(conn, attr, buf, len, offset, (void*)&data, sizeof(data));
        } else {
            return bt_gatt_attr_read(conn, attr, buf, len, offset, 0, 1);
        }
    }

    return 0;
}

/*******************************************************************************
*                                IMU FUNCTIONS                                *
*******************************************************************************/

void imu_callback(IMU::imu_data* imu)
{
    if (streaming)
        imu->print();

    if (!running) return; // Only do this stuff if the data is running

    if (!BLE::connected()) {
    // No BLE Connection
        for (unsigned int i = 0; i < tmpData.size() && imuData.size() < max_readings; i++) {
            IMU::imu_data tmp = tmpData.front();
            imuData.push(tmp);
            tmpData.pop();
        }

        if (imuData.size() >= max_readings) 
            imuData.pop();
        imuData.push(*imu);
    } else {
    // BLE Connection - write to temp storage
        tmpData.push(*imu);
    }

    //BLE::setAdData(imuData.size());
}

void imu_dump_to_term(void)
{
    bool full = (imuData.size() == max_readings);

    while(imuData.size()) {
        IMU::imu_data f = imuData.front();
        imuData.pop();

        f.print();
    }

    if (full)
        printk("WARNING: IMU Data queue was full, "
               "meaning some data points were lost.");

}


/*******************************************************************************
*                                STATE MACHINE                                *
*******************************************************************************/

void stateMachine(void)
{
    sys::msg::MsgData msg = sys::msg::receive();

    switch(msg.tag) {
    case sys::msg::SENSOR_CAPTURE_START:
        running = true;
        break;
    case sys::msg::SENSOR_CAPTURE_STOP:
        running = false;
        imu_dump_to_term();
        break;
    case sys::msg::SENSOR_CAPTURE_STREAM:
        streaming = !streaming;
        break;
    case sys::msg::TIME_SYNC:
        break;
    case sys::msg::NO_MESSAGE:
    default:;
    }
}

/*******************************************************************************
*                                    MAIN                                     *
*******************************************************************************/

void main(void)
{
    BLE::init(imu_data_send);
    IMU::init(DT_LABEL(DT_INST(0, invensense_icm42688)), imu_callback);

    /* Manage the main loop state */
    while (1) {
        stateMachine();
        k_sleep(K_SECONDS(1));
    }
}
