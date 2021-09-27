#include "IMU.hpp"

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <zephyr.h>

#include <memory>

namespace IMU
{
    std::unique_ptr<const device> hw;

    int init(const char *const label)
    {
        hw.reset(device_get_binding(label));

        return 0;
    }
}
