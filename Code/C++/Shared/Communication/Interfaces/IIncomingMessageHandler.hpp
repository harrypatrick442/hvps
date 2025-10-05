#ifndef I_INCOMING_MESSAGE_HANDLER_HPP
#define I_INCOMING_MESSAGE_HANDLER_HPP

#include "../../cJSON/cJSON.h"
class IIncomingMessageHandler {
public:
    virtual ~IIncomingMessageHandler() = default;

    virtual void handleIncomingMessage(cJSON* message, bool& dontDelete) = 0;
};

#endif // I_INCOMING_MESSAGE_HANDLER_HPP