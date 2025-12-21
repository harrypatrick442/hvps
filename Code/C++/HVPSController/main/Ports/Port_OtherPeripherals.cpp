#include "Port_OtherPeripherals.hpp"
#include "Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "JSON/JHelper.hpp"
#include "Messaging/MessageConstants.hpp"
#include "Tasks/TaskFactory.hpp"
#include "../IO/OtherPeripheralsFiberOpticDuplexChannel.hpp"
#include "Generated/Messages/SendStateToIndicateMessage.hpp"
#include "Generated/Messages/IndicateStateMessage.hpp"
#include "Generated/Messages/IndicateStateRequest.hpp"
#include "Generated/Messages/IndicateStateResponse.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "Timing/Delay.hpp"
#include "cJSON/cJSON.h"
#include <memory>
#include <cstring>
#include "Macros/GetFileName.hpp"
const char* Port_OtherPeripherals::getTag() {return GET_FILE_NAME;}
Port_OtherPeripherals::Port_OtherPeripherals(HighSpeedCore& highSpeedCore)
:
_highSpeedCore(highSpeedCore),
_TOSLINKDuplexChannel(new OtherPeripheralsFiberOpticDuplexChannel()),
_ticketedSender(
			[this](cJSON* msg){
				_TOSLINKDuplexChannel->sendMessage(msg, true);
		}){
    _TOSLINKDuplexChannel->setIncomingMessageHandler(this);
	_TOSLINKDuplexChannel->startAsNewNonPriorityTask();
	_messageSender = _TOSLINKDuplexChannel;
	_eventConnectionHighSpeedCoreOnSystemStateChanged = _highSpeedCore.onSystemStateChanged.addHandler(
		[this](SystemState systemState){
			this->sendIndicateStateMessage();
		}
	);
	TaskFactory::createNonPriorityTask(
		[&](){
			sendIndicateStateMessage();
			Delay::ms(200);
			sendIndicateStateMessage();
		}, 
		"sendIndicateStateMessage"
	);
}
Port_OtherPeripherals::~Port_OtherPeripherals() noexcept
{
    delete _TOSLINKDuplexChannel;
	_TOSLINKDuplexChannel = nullptr; 
}
void Port_OtherPeripherals::handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity){
	if(_messageSender==nullptr){
        LOG_ERROR("_messageSender was null. You must set it with setMessageSender");
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
		LOG_INFO("got SendStateToIndicateMessage");
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
	_messageSender->sendMessage(jsonMessage);
}
bool Port_OtherPeripherals::sendIndicateStateRequest(){
	SystemState systemState = _highSpeedCore.getActualSystemState();
	IndicateStateRequest indicateStateRequest((int32_t)systemState);
	cJSON* jsonRequest = indicateStateRequest.toJSON();
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
	JHelper::addInt32(obj, MessageConstants::TARGET_KEY, target);
}
