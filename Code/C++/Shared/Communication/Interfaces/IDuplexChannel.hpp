#pragma once

#include "IMessageSender.hpp"
#include "IMessageReceiver.hpp"
#include "IChannelEvents.hpp"

class IDuplexChannel : public IMessageSender, public IMessageReceiver, public virtual IChannelEvents {
public:
    virtual ~IDuplexChannel() = default;
};
