#include "msg.hpp"

#include <lib/static_queue.h>

namespace sys { namespace msg {

static_queue<MsgData, 32> msgQueue; /**< The messages to dispatch. */

/**
 * Dispatches a message to the main loop.
 *
 * @param[in] tag the message tag
 * @param[in] data message data to attach. Set to nullptr (default) for no
 * attached data.
 * @param[in] len length of data in bytes.
 */
void dispatch(Tag tag, void* data, uint32_t len){
    MsgData newMsg;

    newMsg.tag = tag;

    if (data == nullptr || data == NULL) {
    // If there is no message data
        newMsg.data = nullptr;
        newMsg.dataLen = 0;
    } else if (len > 0) {
    // If there is data pointed to
        newMsg.data = malloc(len);
        if (newMsg.data != NULL) {
            newMsg.dataLen = len;
            memcpy(newMsg.data, data, len);
        } else {
            newMsg.data = nullptr;
            newMsg.dataLen = 0;
        }
    }

    msgQueue.push(std::move(newMsg)); // Add the message to the queue
}

/**
 * Pulls a message from the queue.
 * @warning should only be used in the main loop
 *
 * @return the first message in the queue
 */
MsgData receive(void)
{
    MsgData data;

    if (msgQueue.size()) {
        data = msgQueue.front();
        msgQueue.pop();
    } else {
        data.tag = NO_MESSAGE;
        data.data = nullptr;
        data.dataLen = 0;
    }

    return data;
}

} /* namespace msg */ } /* namespace sys */
