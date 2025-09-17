#ifndef IMESSAGESENDER_HPP
#define IMESSAGESENDER_HPP
#include "../../cJSON/cJSON.h"

class IMessageSender {
public:
    virtual ~IMessageSender() = default;

    // Method to send a message using std::string
    virtual void sendMessage(cJSON* message, bool deleteMessageAfter = true) = 0;
};

#endif // IMESSAGESENDER_HPP