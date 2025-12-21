#include "Port_FiberOpticChannel1.hpp"
#include "../NonVolatileState.hpp"
#include "Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "JSON/JHelper.hpp"
#include "../IO/Inputs.hpp"
#include "Messaging/MessageConstants.hpp"
#include <cstring>
#include <memory>
Port_FiberOpticChannel1::Port_FiberOpticChannel1()
:
_fiberOpticChannel_1(),
_ticketedSender(
			[&](cJSON* msg){
				_fiberOpticChannel_1.sendMessage(msg, true);
		}){
    _fiberOpticChannel_1.setIncomingMessageHandler(this);
	_messageSender = &_fiberOpticChannel_1;
	_fiberOpticChannel_1.startAsNewNonPriorityTask();
}
//TOREMOVE
bool Port_FiberOpticChannel1::setVoltageThreshold(float voltage){
	SetVoltageThresholdRequest request(voltage);
	std::shared_ptr<cJSON> response = _ticketedSender.send(request.toJSON(), TIMEOUT);
	if(response==nullptr){
		return false;
	}
	JHelper::printJsonKeysAndValues(response.get());
	return true;
}
void Port_FiberOpticChannel1::handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity){
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
		LOG_INFO("Got ticketed");
		_ticketedSender.handleTicketedMessage(message, type);
		dontDelete = true;
		return;
	}
	if(strcmp(type, SetVoltageThresholdRequest::TYPE) == 0){
		LOG_INFO("Got set voltage");
		handleSetVoltageThresholdRequest(message);
		return;
	}
	if(strcmp(type, GetVoltageRequest::TYPE) == 0){
		LOG_INFO("Got get voltage");
		handleGetVoltageRequest(message);
		return;
	}
}
void Port_FiberOpticChannel1::handleSetVoltageThresholdRequest(cJSON* message){
	std::shared_ptr<SetVoltageThresholdRequest> request= SetVoltageThresholdRequest::fromJSON(message);
	float voltage = request->getVoltage();
	uint64_t ticket = request->getTicket();
	NonVolatileState::setVoltageThreshold(voltage);
	
	//LOG_INFO("handleSetVoltageThresholdRequest");
	
	//LOG_INFO("ticket from request was %" PRIu64 "", ticket);
	SetVoltageThresholdResponse* response = new SetVoltageThresholdResponse(ticket);
	_messageSender->sendMessage(response->toJSON());
	//LOG_INFO("handleSetVoltageThresholdRequest sent response");
	delete response;
}
void Port_FiberOpticChannel1::handleGetVoltageRequest(cJSON* message){
	std::shared_ptr<GetVoltageRequest> request = GetVoltageRequest::fromJSON(message);
	uint64_t ticket = request->getTicket();
	Inputs::selectADCVoltageDividerInputAsChannel();
	float voltage = Inputs::getADCVoltage();
	GetVoltageResponse* response = new GetVoltageResponse(voltage, ticket);
	_messageSender->sendMessage(response->toJSON());
	delete response;
}

