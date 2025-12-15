#include "Port_VoltageFeedbackBase.hpp"
#include "Logging/Log.hpp"
#include "System/Aborter.hpp"
#include "JSON/JHelper.hpp"
#include "Messaging/MessageConstants.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Generated/Messages/SetVoltageThresholdRequest.hpp"
#include "Generated/Messages/SetVoltageThresholdResponse.hpp"
#include "Generated/Messages/GetVoltageThresholdRequest.hpp"
#include "Generated/Messages/GetVoltageThresholdResponse.hpp"
#include "Generated/Messages/GetVoltageRequest.hpp"
#include "Generated/Messages/GetVoltageResponse.hpp"
#include "Generated/Messages/SetForceVoltageThresholdReachedFeedbackRequest.hpp"
//#include "Generated/Messages/SetForceVoltageThresholdReachedFeedbackResponse.hpp"
#include "Generated/Messages/GreetingRequest.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/VoltageMessage.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Generated/Messages/ClearLoggedErrorsMessage.hpp"
#include <memory>
#include <cstring>
Port_VoltageFeedbackBase::Port_VoltageFeedbackBase(TOSLINKDuplexChannel* toslinkDuplexChannel)
:
_TOSLINKDuplexChannel(toslinkDuplexChannel),
_ticketedSender(
			[this](cJSON* msg){
				_TOSLINKDuplexChannel->sendMessage(msg, true);
		}){
    _TOSLINKDuplexChannel->setIncomingMessageHandler(this);
	_TOSLINKDuplexChannel->startAsNewNonPriorityTask();
	_messageSender = _TOSLINKDuplexChannel;
}
Port_VoltageFeedbackBase::~Port_VoltageFeedbackBase() noexcept
{
    delete _TOSLINKDuplexChannel;
	_TOSLINKDuplexChannel = nullptr; 
}
bool Port_VoltageFeedbackBase::setVoltageThreshold(double voltage){
	SetVoltageThresholdRequest request(voltage);
	std::shared_ptr<cJSON> response = _ticketedSender.send(request.toJSON(), TIMEOUT);
	if(response==nullptr){
		return false;
	}
	return true;
}
bool Port_VoltageFeedbackBase::getVoltageThreshold(double& voltage) {
    voltage = -1.0;

    GetVoltageThresholdRequest request;   // NOTE: not request()
    std::shared_ptr<cJSON> jsonResponse = _ticketedSender.send(request.toJSON(), TIMEOUT);
    if (!jsonResponse) {
        return false;
    }
	CleanupBucket cleanupBucket;
	GetVoltageThresholdResponse* response = GetVoltageThresholdResponse::fromJSON(jsonResponse.get(), cleanupBucket);
    voltage = response->getVoltage();
    return true;
}
bool Port_VoltageFeedbackBase::getVoltage(double& voltage) {
    voltage = -1.0;
    GetVoltageRequest request;   // NOTE: not request()
    std::shared_ptr<cJSON> jsonResponse = _ticketedSender.send(request.toJSON(), TIMEOUT);
    if (!jsonResponse) {
        return false;
    }
	CleanupBucket cleanupBucket;
	GetVoltageResponse* response = GetVoltageResponse::fromJSON(jsonResponse.get(), cleanupBucket);
    voltage = response->getVoltage();
    return true;
}
bool Port_VoltageFeedbackBase::setForceThresholdReachedFeedback(bool force){
    SetForceVoltageThresholdReachedFeedbackRequest request(force);
    std::shared_ptr<cJSON> jsonResponse = _ticketedSender.send(request.toJSON(), TIMEOUT);
    if (!jsonResponse) {
        return false;
    }
    return true;
}
void Port_VoltageFeedbackBase::handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity){
	if(_messageSender==nullptr){
        Log::Error(getTag(), "_messageSender was null. You must set it with setMessageSender");
		return;
	}
	bool success = true;
	char* type = JHelper::getString(message, "tpe", success);
	if (!success) {
		return;
	}
	if(strcmp(type, MessageConstants::TYPE_TICKETED_VALUE) == 0){
		//Log::Info(getTag(), "Got ticketed");
		_ticketedSender.handleTicketedMessage(message, type);
		dontDelete = true;
		return;
	}
	if(strcmp(type, VoltageMessage::TYPE) == 0){
		handleVoltageMessage(message);
		return;
	}
	if(strcmp(type, GreetingMessage::TYPE) == 0){
		handleGreetingMessage(message);
		return;
	}
}
void Port_VoltageFeedbackBase::handleVoltageMessage(cJSON* message){
	CleanupBucket cleanupBucket;
	VoltageMessage* voltageMessage = VoltageMessage::fromJSON(message, cleanupBucket);
	double voltage = voltageMessage->getVoltage();
	onGotVoltage.dispatch(voltage);
}
GreetingResponse* Port_VoltageFeedbackBase::greet(
	CleanupBucket& cleanupBucket){
    GreetingRequest request;
    std::shared_ptr<cJSON> jsonResponse = 
		_ticketedSender.send(request.toJSON(), TIMEOUT);
    if (!jsonResponse) {
		return nullptr;
    };
	return GreetingResponse::fromJSON(jsonResponse.get(), cleanupBucket);
}
void Port_VoltageFeedbackBase::handleGreetingMessage(cJSON* message){
	CleanupBucket cleanupBucket;
	GreetingMessage* greetingMessage = GreetingMessage::fromJSON(
		message, cleanupBucket);
	if(greetingMessage==nullptr){
		return;
	}
	onGotGreetingMessage.dispatch(greetingMessage);
}
void Port_VoltageFeedbackBase::sendClearLoggedErrors(){
	ClearLoggedErrorsMessage message;
	_messageSender->sendMessage(message.toJSON());
}