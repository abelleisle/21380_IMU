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
    /* The function called when data is recieved */
    using RecvCallback       = std::function<ssize_t(struct bt_conn*, const struct bt_gatt_attr*,
                                                     const void*, uint16_t, uint16_t uint8_t)>;
    /* The function called when data needs to be sent */
    using SendCallback       = std::function<ssize_t(struct bt_conn*, const struct bt_gatt_attr*,
                                                     void*, uint16_t, uint8_t)>;
    /* Callback for when bluetooth connection occurs */
    using ConnectCallback    = std::function<void(struct bt_conn*, uint8_t)>;
    /* Callback for when bluetooth disconnect occurs */
    using DisconnectCallback = std::function<void(struct bt_conn*, uint8_t)>;

    /* Call back selection. This is passed to the init function */
    struct Callback_t {
        SendCallback send             = nullptr;
        RecvCallback recv             = nullptr;
        ConnectCallback connect       = nullptr;
        DisconnectCallback disconnect = nullptr;
    };

    int init(Callback_t*);
    void setAdData(uint32_t data);
}
