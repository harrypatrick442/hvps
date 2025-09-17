#ifndef ICHANNEL_HPP
#define ICHANNEL_HPP

#include "IMessageSender.hpp"
#include "IMessageReceiver.hpp"

class IChannel : public IMessageSender, public IMessageReceiver {
public:
    virtual ~IChannel() = default;
};

#endif // ICHANNEL_HPP
