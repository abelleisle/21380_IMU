#include "BLE.hpp"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <zephyr/types.h>
#include <stddef.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

namespace BLE
{
    static struct bt_data ad[2] = {
        BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x12, 0x34),
        BT_DATA(BT_DATA_SVC_DATA16, 0, 1)
    };

    /* Set Scan Response data */
    static const struct bt_data sd[] = {
        BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    };

    static void ready_cb(int err);

    void setAdData(uint8_t *data, int size)
    {
        ad[1] = BT_DATA(BT_DATA_SVC_DATA16, data, size);
        bt_le_adv_stop();
        int err;
        /* Start advertising */
        const struct bt_le_adv_param param = 
            BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_USE_IDENTITY,
                                 BT_GAP_ADV_FAST_INT_MIN_2,
                                 BT_GAP_ADV_FAST_INT_MAX_2,
                                 NULL);

        err = bt_le_adv_start(&param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
        if (err) {
            printk("Advertising failed to start (err %d)\n", err);
            return;
        }
    }

    int init(void)
    {
        int err;

        err = bt_enable(ready_cb);

        if (err) {
            printk("Bluetooth initialization failed... Error: %d\n", err);
            return err;
        }

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
            BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_USE_IDENTITY,
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
}
