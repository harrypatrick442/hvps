#include "Port_FiberOpticChannel1.hpp"
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include "../JSON/JHelper.hpp"
#include "../IO/AnalogueInputs.hpp"
#include "../Messaging/MessageConstants.hpp"
#include <cstring>
const char* Port_FiberOpticChannel1::TAG = "Port_FiberOpticChannel1";
Port_FiberOpticChannel1* Port_FiberOpticChannel1::_instance = nullptr;
Port_FiberOpticChannel1& Port_FiberOpticChannel1::initialize(State* state){
    if(_instance != nullptr){
        Aborter::safeAbort(TAG, "Already initialized");
        return *_instance;
    }
    _instance = new Port_FiberOpticChannel1(state);
    return *_instance;
}
Port_FiberOpticChannel1::Port_FiberOpticChannel1(State* state)
:_state(state),
_fiberOpticChannel_1(),
_ticketedSender(
			[&](cJSON* msg){
				_fiberOpticChannel_1.sendMessage(msg, true);
		}){
    _fiberOpticChannel_1.setIncomingMessageHandler(this);
	_fiberOpticChannel_1.startAsNewNonPriorityTask();
	_messageSender = &_fiberOpticChannel_1;
}
bool Port_FiberOpticChannel1::setVoltageThreshold(double voltage){
	SetVoltageThresholdRequest request(voltage);
	cJSON* response = _ticketedSender.send(request.toJSON(), 1000);
	if(response==nullptr){
		return false;
	}
	JHelper::printJsonKeysAndValues(response);
	cJSON_Delete(response);
	return true;
}
void Port_FiberOpticChannel1::handleIncomingMessage(cJSON* message, bool& dontDelete){
	if(_messageSender==nullptr){
        Log::Error(TAG, "_messageSender was null. You must set it with setMessageSender");
		return;
	}
	bool success = true;
	//Log::Info(TAG, "handleIncomingMessage");
	char* type = JHelper::getString(message, "tpe", success);
	if (!success) {
		return;
	}
	//Log::Info(TAG, "handleIncomingMessage type: %s", type);
	/*if(strcmp(type, "p") == 0){
		Log::Info(TAG, "Got ping");
		_messageSender->sendMessage(message);
		return;
	}*/
	
	if(strcmp(type, MessageConstants::TYPE_TICKETED_VALUE) == 0){
		Log::Info(TAG, "Got ticketed");
		_ticketedSender.handleTicketedMessage(message, type);
		dontDelete = true;
		return;
	}
	if(strcmp(type, SetVoltageThresholdRequest::TYPE) == 0){
		Log::Info(TAG, "Got set voltage");
		handleSetVoltageThresholdRequest(SetVoltageThresholdRequest::fromJSON(message));
		return;
	}
	if(strcmp(type, GetVoltageRequest::TYPE) == 0){
		Log::Info(TAG, "Got get voltage");
		handleGetVoltageRequest(GetVoltageRequest::fromJSON(message));
		return;
	}
}
void Port_FiberOpticChannel1::handleSetVoltageThresholdRequest(SetVoltageThresholdRequest* request){
	double voltage = request->getVoltage();
	uint64_t ticket = request->getTicket();
	delete request;
	_state->setVoltageThreshold(voltage);
	
	//Log::Info(TAG, "handleSetVoltageThresholdRequest");
	
	Log::Info(TAG, "ticket from request was %" PRIu64 "", ticket);
	SetVoltageThresholdResponse* response = new SetVoltageThresholdResponse(ticket);
	_messageSender->sendMessage(response->toJSON());
	//Log::Info(TAG, "handleSetVoltageThresholdRequest sent response");
	delete response;
}
void Port_FiberOpticChannel1::handleGetVoltageRequest(GetVoltageRequest* request){
	uint64_t ticket = request->getTicket();
	delete request;
	double voltage = AnalogueInputs::getVoltage();
	GetVoltageResponse* response = new GetVoltageResponse(voltage, ticket);
	_messageSender->sendMessage(response->toJSON());
	delete response;
}

