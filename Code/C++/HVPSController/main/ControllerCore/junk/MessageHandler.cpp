#include "./MessageHandler.hpp"
#include "../Logging/Log.hpp"
#include "../JSON/JHelper.hpp"
#include <cstring>
MessageHandler* MessageHandler::_instance = nullptr;
MessageHandler& MessageHandler::initialize(){
    if(_instance != nullptr){
        Log::Error("MessageHandler", "MessageHandler already initialized");
        abort();
        return *_instance;
    }
    _instance = new MessageHandler();
    return *_instance;
}
MessageHandler& MessageHandler::getInstance(){
    if(_instance == nullptr){
        Log::Error("MessageHandler", "MessageHandler not initialized. Call initialize() first.");
        abort();
    }
    return *_instance;
}
void MessageHandler::setMessageSender(IMessageSender* messageSender){
    _messageSender = messageSender;
}
void MessageHandler::handleIncomingMessage(cJSON* message){
	if(_messageSender==nullptr){
        Log::Error("MessageHandler", "_messageSender was null. You must set it with setMessageSender");
		return;
	}
	bool success;
	char* type = JHelper::getString(message, "tpe", success);
	if (!success) {
		return;
	}
	if(strcmp(type, "p") == 0){
		_messageSender->sendMessage(message);
	}
}
