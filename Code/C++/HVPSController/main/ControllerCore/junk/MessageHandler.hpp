#ifndef MessageHandler_hpp
#define MessageHandler_hpp
#include "../Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "../Communication/Interfaces/IMessageSender.hpp"
#include "../cJSON/cJSON.h"
class MessageHandler : public IIncomingMessageHandler{
    public :
        MessageHandler& initialize();
        MessageHandler& getInstance();
        void setMessageSender(IMessageSender* messageSender);
        void handleIncomingMessage(cJSON* json) override;
    private:
        static MessageHandler* _instance;
        IMessageSender* _messageSender;
};

#endif // MessageHandler_hpp