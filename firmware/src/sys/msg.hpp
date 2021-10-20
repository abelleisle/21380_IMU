#pragma once

#include <zephyr/types.h>

namespace sys { namespace msg {

enum Tag {
    SENSOR_CAPTURE_START,
    SENSOR_CAPTURE_STOP,
    TIME_SYNC,
    NO_MESSAGE
};

struct MsgData {
    Tag tag;
    void* data;
    uint32_t dataLen;
};

void dispatch(Tag tag, void* data = nullptr, uint32_t len = 0);
MsgData receive(void);

} /* namespace msg */ } /* namespace sys */
