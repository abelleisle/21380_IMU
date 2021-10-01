#pragma once

#include <stdint.h>
#include <functional>

namespace IMU
{
    int init(const char* const label);
    int setCallback(std::function<void()> _cb);
}
