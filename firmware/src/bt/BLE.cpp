#include "BLE.hpp"

#include <bluetooth/bluetooth.h>

namespace BLE
{
    static void ready_cb(int err);

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

    }
}
