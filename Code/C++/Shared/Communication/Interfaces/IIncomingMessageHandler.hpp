#ifndef I_INCOMING_MESSAGE_HANDLER_HPP
#define I_INCOMING_MESSAGE_HANDLER_HPP

#include "../../cJSON/cJSON.h"
#include "../Enums/MessageIntegrity.hpp"
class IIncomingMessageHandler {
public:
    virtual ~IIncomingMessageHandler() = default;

    virtual void handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity) = 0;
};

#endif // I_INCOMING_MESSAGE_HANDLER_HPP