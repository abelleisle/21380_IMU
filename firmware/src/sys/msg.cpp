#include "msg.hpp"

#include <queue>

namespace sys { namespace msg {

std::queue<MsgData> msgQueue;

void dispatch(Tag tag, void* data, uint32_t len){
    MsgData newMsg;

    newMsg.tag = tag;

    if (data == nullptr || data == NULL) {
        newMsg.data = nullptr;
        newMsg.dataLen = 0;
    } else {
        newMsg.data = ::operator new(len);
        newMsg.dataLen = len;
    }

    msgQueue.push(std::move(newMsg));
}

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
