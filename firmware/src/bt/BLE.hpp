#pragma once

#include <zephyr/types.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/conn.h>
#include <functional>

namespace BLE
{
    using RecvCallback = std::function<ssize_t(struct bt_conn*, const struct bt_gatt_attr*,
                                               const void*, uint16_t, uint16_t uint8_t)>;

    using SendCallback = std::function<ssize_t(struct bt_conn*, const struct bt_gatt_attr*,
                                               void*, uint16_t, uint8_t)>;

    int init(SendCallback);
    void setAdData(uint32_t data);

    bool connected();
}
