#include "Port_FiberOpticChannel1.hpp"
#include "Logging/Log.hpp"
#include "JSON/JHelper.hpp"
#include "Messaging/MessageConstants.hpp"
#include "Generated/Messages/IndicateStateRequest.hpp"
#include "Generated/Messages/IndicateStateResponse.hpp"
#include "Generated/Messages/IndicateStateMessage.hpp"
#include "Generated/Messages/SendStateToIndicateMessage.hpp"
#include "Tasks/TaskFactory.hpp"
#include <cstring>
#include <memory>
Port_FiberOpticChannel1::Port_FiberOpticChannel1(
	uint32_t subsystemIdentifier,
	ISystemStateIndicator& systemStateIndicator
):
_subsystemIdentifier(subsystemIdentifier),
_systemStateIndicator(systemStateIndicator),
_fiberOpticChannel_1(),
_ticketedSender(
			[&](cJSON* msg){
				_fiberOpticChannel_1.sendMessage(msg, true);
		}){
    _fiberOpticChannel_1.setIncomingMessageHandler(this);
	_messageSender = &_fiberOpticChannel_1;
	_fiberOpticChannel_1.startAsNewNonPriorityTask();
	TaskFactory::createNonPriorityTask(
		[&](){
			sendSendStateToIndicate();
			vTaskDelete(NULL);
		}, 
		"sendSendStateToIndicate"
	);
}
void Port_FiberOpticChannel1::handleIncomingMessage(cJSON* message, bool& dontDelete){
	if(_messageSender==nullptr){
        Log::Error(TAG, "_messageSender was null. You must set it with setMessageSender");
		return;
	}
	bool success = true;
	uint32_t target = 
		JHelper::getUInt32(message, MessageConstants::TYPE_KEY, success);
	if (!success || (target!=_subsystemIdentifier)) {
		return;
	}
	char* type = JHelper::getString(message, MessageConstants::TYPE_KEY, success);
	if (!success) {
		return;
	}
	if(strcmp(type, MessageConstants::TYPE_TICKETED_VALUE) == 0){
		Log::Info(TAG, "Got ticketed");
		_ticketedSender.handleTicketedMessage(message, type);
		dontDelete = true;
		return;
	}
	if(strcmp(type, IndicateStateRequest::TYPE) == 0){
		Log::Info(TAG, "Got indicate state request");
		handleIndicateStateRequest(message);
		return;
	}
	if(strcmp(type, IndicateStateMessage::TYPE) == 0){
		Log::Info(TAG, "Got indicate state message");
		handleIndicateStateMessage(message);
		return;
	}
}
void Port_FiberOpticChannel1::handleIndicateStateRequest(cJSON* message){
	CleanupBucket cleanupBucket;
	IndicateStateRequest* request= IndicateStateRequest::fromJSON(message, cleanupBucket);
	_systemStateIndicator.indicateState(static_cast<SystemState>(request->getSystemState()));
	IndicateStateResponse response(true, request->getTicket());
	_messageSender->sendMessage(response.toJSON());
}
void Port_FiberOpticChannel1::handleIndicateStateMessage(cJSON* message){
	CleanupBucket cleanupBucket;
	IndicateStateRequest* indicateStateMessage = IndicateStateRequest::fromJSON(message, cleanupBucket);
	_systemStateIndicator.indicateState(static_cast<SystemState>(indicateStateMessage->getSystemState()));
}
void Port_FiberOpticChannel1::sendSendStateToIndicate(){
	CleanupBucket cleanupBucket;
	SendStateToIndicateMessage request;
	_messageSender->sendMessage(request.toJSON());
}

