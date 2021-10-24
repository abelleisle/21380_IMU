#include "BLE.hpp"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/conn.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdint.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/*******************************************************************************
*                              UUID DECLARATIONS                              *
*******************************************************************************/

/* Sensor data service */
#define SENSOR_SRV_DECLARE BT_UUID_128_ENCODE(0x1E47B200, 0xF208, 0x4D20, 0xBBC7, 0xDAC31E042204)
static struct bt_uuid_128 sensor_svc_uuid = BT_UUID_INIT_128(SENSOR_SRV_DECLARE);

/* Sensor Attribute */
#define SENSOR_ATT_DECLARE BT_UUID_128_ENCODE(0x1E47B201, 0xF208, 0x4D20, 0xBBC7, 0xDAC31E042204)
static struct bt_uuid_128 sensor_att_uuid = BT_UUID_INIT_128(SENSOR_ATT_DECLARE);

namespace BLE
{
    /**************************************************************************
    *                               VARIABLES                                *
    **************************************************************************/
    
    /* Set Advertising data */
    static struct bt_data ad[4] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
        BT_DATA_BYTES(BT_DATA_UUID128_ALL, SENSOR_SRV_DECLARE),
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_CTS_VAL)),
        BT_DATA_BYTES(BT_DATA_SVC_DATA32, 0x00, 0x00, 0x00, 0x00)
    };

    /* Set Scan Response data */
    static const struct bt_data sd[] = {
        BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    };

    SendCallback send;
    static void ready_cb(int err);
    static void connected_cb(struct bt_conn *conn, uint8_t err);
    static void disconnected_cb(struct bt_conn *conn, uint8_t reason);
    static ssize_t write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                            const void *buf, uint16_t len, uint16_t offset, uint8_t flags);
    static ssize_t read_cb (struct bt_conn *conn, const struct bt_gatt_attr *attr,
                            void *buf, uint16_t len, uint16_t offset);

    bool _connected = false;

    /**************************************************************************
    *                          SERVICE DECLARATIONS                          *
    **************************************************************************/

    BT_GATT_SERVICE_DEFINE(sensor_svc, 
        BT_GATT_PRIMARY_SERVICE(&sensor_svc_uuid),
        BT_GATT_CHARACTERISTIC(&sensor_att_uuid.uuid,
                               BT_GATT_CHRC_READ | BT_GATT_CHRC_INDICATE,
                               BT_GATT_PERM_READ,
                               read_cb, write_cb, 0
        )
    );

    
    static struct bt_conn_cb conn_callbacks = {
        .connected = connected_cb,
        .disconnected = disconnected_cb
    };

    /**************************************************************************
    *                               FUNCTIONS                                *
    **************************************************************************/

    void setAdData(uint32_t data)
    {
        ad[3] = bt_data{.type = BT_DATA_SVC_DATA32,
                        .data_len = 4,
                        .data = (uint8_t*)(&data)};

        /* Update advertising data */
        int err = bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
        if (err) {
            printk("Advertising failed to start (err %d)\n", err);
            return;
        }
    }

    int init(SendCallback send_cb)
    {
        int err;

        bt_conn_cb_register(&conn_callbacks);

        err = bt_enable(ready_cb);

        if (err) {
            printk("Bluetooth initialization failed... Error: %d\n", err);
            return err;
        }

        send = send_cb;

        return 0;
    }

    /**
     * BLE Ready callback
     * Once BLE is enabled, this function is called to perform BLE post-startup
     * routine.
     *
     * @param[int] err Any error codes that occurred during BLE startup
     */
    static void ready_cb(int err)
    {
        char addr_s[BT_ADDR_LE_STR_LEN];
        bt_addr_le_t addr = {0};
        size_t count = 1;

        if (err) {
            printk("Bluetooth init failed (err %d)\n", err);
            return;
        }

        printk("Bluetooth initialized\n");

        /* Start advertising */
        const struct bt_le_adv_param param = 
            BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_USE_IDENTITY | BT_LE_ADV_OPT_CONNECTABLE,
                                 BT_GAP_ADV_FAST_INT_MIN_2,
                                 BT_GAP_ADV_FAST_INT_MAX_2,
                                 NULL);

        err = bt_le_adv_start(&param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
        if (err) {
            printk("Advertising failed to start (err %d)\n", err);
            return;
        }


        /* For connectable advertising you would use
        * bt_le_oob_get_local().  For non-connectable non-identity
        * advertising an non-resolvable private address is used;
        * there is no API to retrieve that.
        */

        bt_id_get(&addr, &count);
        bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

        printk("Beacon started, advertising as %s\n", addr_s);
    }

    bool connected()
    {
        return _connected;
    }

    /**************************************************************************
    *                               CALLBACKS                                *
    **************************************************************************/

    static void print_conn_info(struct bt_conn *conn)
    {
        struct bt_conn_info info;
        int err = bt_conn_get_info(conn, &info);
        if (err)
        {
            printk("Could not get connection information!\n");
            return;
        }
        printk("Connection Information:\n");
        printk("* Role: ");
        if (info.role == BT_CONN_ROLE_PERIPHERAL)
            printk("Master.\n");
        else
            printk("Slave.\n");
        if(info.type & BT_CONN_TYPE_LE)
        {
            printk("* Connection Type: LE\n");
#if defined(CONFIG_BT_USER_PHY_UPDATE)
            /* -> bluetooth/gap.h
            * LE PHY types
            * enum {
            * // Convenience macro for when no PHY is set.
            * BT_GAP_LE_PHY_NONE                    = 0,
            * // LE 1M PHY
            * BT_GAP_LE_PHY_1M                      = BIT(0),
            * //LE 2M PHY
            * BT_GAP_LE_PHY_2M                      = BIT(1),
            * // LE Coded PHY
            * BT_GAP_LE_PHY_CODED                   = BIT(2),
            * };
            */
            const struct bt_conn_le_phy_info *phy_info = info.le.phy;
            printk("  PHY: TX: 0x%02X, RX: 0x%02X.\n", phy_info->tx_phy, phy_info->rx_phy);
#endif /* defined(CONFIG_BT_USER_PHY_UPDATE) */

#if defined(CONFIG_BT_USER_DATA_LEN_UPDATE)
            const struct bt_conn_le_data_len_info *data_len_info = info.le.data_len;
            printk("  Data Length: TX Max length: %d, TX Max time: %dus, RX Max length: %d, RX Max time: %dus.\n",
                        data_len_info->tx_max_len, data_len_info->tx_max_time,
                        data_len_info->rx_max_len, data_len_info->rx_max_time);
#endif /* defined(CONFIG_BT_USER_DATA_LEN_UPDATE) */
        }
    }

    static void connected_cb(struct bt_conn *conn, uint8_t err)
    {
        _connected = true;
        if (err) {
            printk("Connection failed (reason 0x%02x)\n", err);
        } else {
            printk("Connected\n");
            print_conn_info(conn);
        }
    }

    static void disconnected_cb(struct bt_conn *conn, uint8_t reason)
    {    
        printk("Disconnected (reason 0x%02x)\n", reason);
        _connected = false;
    }

    static ssize_t write_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                            const void *buf, uint16_t len, uint16_t offset, uint8_t flags)
    {
        return 0;
    }

    static ssize_t read_cb (struct bt_conn *conn, const struct bt_gatt_attr *attr,
                            void *buf, uint16_t len, uint16_t offset)
    {
        return send(conn, attr, buf, len, offset);
    }
}
