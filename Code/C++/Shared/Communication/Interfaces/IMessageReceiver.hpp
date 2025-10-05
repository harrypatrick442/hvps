#ifndef I_MESSAGE_RECEIVER_HPP
#define I_MESSAGE_RECEIVER_HPP

#include "IIncomingMessageHandler.hpp"

class IMessageReceiver {
public:
    virtual ~IMessageReceiver() = default;
    virtual void setIncomingMessageHandler(IIncomingMessageHandler* messageHandler) = 0;
};

#endif // I_MESSAGE_RECEIVER_HPP