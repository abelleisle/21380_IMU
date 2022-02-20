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

#include <lib/static_queue.h>

#include <stdio.h>
#include <cstring>
#include <queue>

/**
 * TODO: remove global variables
 * TODO: change IMU callbacks to class
 * TODO: change BLE callbacks to class
 */

/*******************************************************************************
*                       GLOBALS (EWWWWWWWW MUST REMOVE)                       *
*******************************************************************************/

static_queue<IMU::imu_data, 512> imuData; /**< IMU data storage queue. */

/* Debug state modification */
bool running = true;    /**< Whether or not IMU data is recorded */
bool streaming = false; /**< Print IMU recording to shell */

/*******************************************************************************
*                                BT CALLBACKS                                 *
*******************************************************************************/

/**
 * Current BLE state
 */
enum BLE_State_t {
    SEND_SIZE,
    SEND_PACKET,
    SEND_EMPTY,
} ble_state;

uint32_t ble_size; /**< How many IMU packets are available to send */

/**
 * Function to send data to master.
 *
 * @param[in] conn current BLE connection
 * @param[in] attr BLE attribute being written to
 * @param[out] buf the buffer to write data to
 * @param[in] len how many bytes the master requested
 * @param[in] offset data offset in 'buf'
 *
 * @return How many bytes were written to the master
 */
ssize_t imu_data_send(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                    void *buf, uint16_t len, uint8_t offset)
{
    ssize_t sz;

    switch (ble_state) {
    case SEND_SIZE:
        ble_size = imuData.size();

        sz = bt_gatt_attr_read(conn, attr, buf, len, offset,
                               (void*)&ble_size, sizeof(ble_size));

        
        if (ble_size)
            ble_state = SEND_PACKET;
        else
            ble_state = SEND_EMPTY;
        break;
    case SEND_PACKET:
        if (ble_size) {
            IMU::imu_data data = imuData.front();
            sz = bt_gatt_attr_read(conn, attr, buf, len, offset,
                                   (void*)&data, sizeof(data));
            if (!--ble_size)
                ble_state = SEND_EMPTY;
            break;
        }
    case SEND_EMPTY:
    default:
        sz =  bt_gatt_attr_read(conn, attr, buf, len, offset, 0, 1);
    }

    return sz;
}

/**
 * BLE connection callback
 *
 * @param[in] conn current BLE connection
 * @param[in] err to propagate, currently not handled
 */
void ble_connect(struct bt_conn* conn, uint8_t err)
{
    ble_state = SEND_SIZE;
}

/**
 * BLE disconnect callback
 *
 * @param[in] conn current BLE connection
 * @param[in] err to propagate, currently not handled
 */
void ble_disconnect(struct bt_conn* conn, uint8_t reason)
{
    ble_state = SEND_SIZE;
}

// BLE callbacks
BLE::Callback_t BLE_Callbacks = {
    .send       = imu_data_send,
    .recv       = nullptr,
    .connect    = ble_connect,
    .disconnect = ble_disconnect
};

/*******************************************************************************
*                                IMU FUNCTIONS                                *
*******************************************************************************/

/**
 * Called whenever IMU data is recorded.
 *
 * @param[in] imu The data that was recorded.
 */
void imu_callback(IMU::imu_data* imu)
{
    if (streaming)
        imu->print();

    if (!running) return; // Only do this stuff if the data is running

    imuData.push(*imu);

    BLE::setAdData(imuData.size());
}

/**
 * Empties IMU queue and prints all data to shell.
 * @warning should only be used in debug mode
 */
void imu_dump_to_term(void)
{
    bool full = (imuData.size() == imuData.max_size());

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

/**
 * Main state machine.
 *
 * Looped forever. As of now this is only used for debug state selection.
 */
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

/**
 * main.
 * Firmware entry point.
 */
void main(void)
{
    BLE::init(&BLE_Callbacks); // Initialize the BLE system

    #if defined(invensense_icm42688)
        // Hardware IMU
        IMU::init(DT_LABEL(DT_INST(0, invensense_icm42688)), imu_callback);
    #else
        // Software IMU
        IMU::init(nullptr, imu_callback);
    #endif

    /* Manage the main loop state */
    while (1) {
        stateMachine();
        k_sleep(K_SECONDS(1));
    }
}
