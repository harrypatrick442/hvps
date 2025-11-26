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
#include "Generated/Messages/GreetingRequest.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/VoltageMessage.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Generated/Messages/IndicateStateMessage.hpp"
#include "Generated/Messages/IndicateStateRequest.hpp"
#include "Generated/Messages/IndicateStateResponse.hpp"
#include <memory>
#include <cstring>
Port_OtherPeripherals::Port_OtherPeripherals()
:
_TOSLINKDuplexChannel(new OutputVoltageFeedbackFiberOpticDuplexChannel()),
_ticketedSender(
			[this](cJSON* msg){
				_TOSLINKDuplexChannel->sendMessage(msg, true);
		}){
    _TOSLINKDuplexChannel->setIncomingMessageHandler(this);
	_TOSLINKDuplexChannel->startAsNewNonPriorityTask();
	_messageSender = _TOSLINKDuplexChannel;
	TaskFactory::createNonPriorityTask(
		[&](){
			sendState();
			vTaskDelete(NULL);
		}, 
		"sendState"
	);
}
Port_OtherPeripherals::~Port_OtherPeripherals() noexcept
{
    delete _TOSLINKDuplexChannel;
	_TOSLINKDuplexChannel = nullptr; 
}
void Port_OtherPeripherals::handleIncomingMessage(cJSON* message, bool& dontDelete){
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
		_ticketedSender.handleTicketedMessage(message, type);
		dontDelete = true;
		return;
	}
	if(strcmp(type, SendStateToIndicateMessage::TYPE) == 0){
		handleSendStateToIndicateMessage();
		return;
	}
}
void Port_OtherPeripherals::handleSendStateToIndicateMessage(){	
	sendIndicateStateMessage();
}
void Port_OtherPeripherals::sendIndicateStateMessage(){
	SystemState systemState = _highSpeedCore.getActualSystemState();
	IndicateStateMessage indicateStateMessage((int32_t)systemState);
	cJSON* jsonMessage = indicateStateMessage.toJSON();
	setTarget(jsonMessage, SubsystemIdentifiers::Peripheral1);
	_channel.sendMessage(jsonMessage);
}
bool Port_OtherPeripherals::sendIndicateStateRequest(){
	SystemState systemState = _highSpeedCore.getActualSystemState();
	IndicateStateRequest indicateStateRequest((int32_t)systemState);
	cJSON* jsonRequest = stateChangedMessage.toJSON();
	setTarget(jsonRequest, SubsystemIdentifiers::Peripheral1);
	std::shared_ptr<cJSON> jsonResponse = _ticketedSender.send(jsonRequest, TIMEOUT);
	if(jsonResponse==nullptr){
		return false;
	}
	CleanupBucket cleanupBucket;
	IndicateStateResponse* response = IndicateStateResponse::fromJSON(jsonResponse.get(), cleanupBucket);
	return response->getSuccess();
}
void Port_OtherPeripherals::setTarget(cJSON* obj, uint32_t target){
	JHelper::addInt32(cJSON* obj, MessageConstants::TARGET_KEY, target);
}
