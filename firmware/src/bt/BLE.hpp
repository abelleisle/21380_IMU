#pragma once

#include <zephyr/types.h>

namespace BLE
{
    int init(void);
    void setAdData(uint32_t data);
}
