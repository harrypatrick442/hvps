#pragma once

#include "IMessageSender.hpp"
#include "IMessageReceiver.hpp"
#include "IChannelEvents.hpp"

class IChannel : public IMessageSender, public IMessageReceiver, public IChannelEvents {
public:
    virtual ~IChannel() = default;
};
