#pragma once

#include <zephyr/types.h>

namespace BLE
{
    int init(void);
    void setAdData(uint8_t *data, int size);
}
