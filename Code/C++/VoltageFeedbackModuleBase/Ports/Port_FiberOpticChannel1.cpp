#include "Port_FiberOpticChannel1.hpp"
#include "../NonVolatileState.hpp"
#include "Logging/Log.hpp"
#include "System/Aborter.hpp"
#include "JSON/JHelper.hpp"
#include "../IO/Inputs.hpp"
#include "Messaging/MessageConstants.hpp"
#include <cstring>
#include <memory>
Port_FiberOpticChannel1::Port_FiberOpticChannel1(
	ThresholdMonitor& thesholdMonitor
)
:
_thesholdMonitor(thesholdMonitor),
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
bool Port_FiberOpticChannel1::setVoltageThreshold(double voltage){
	SetVoltageThresholdRequest request(voltage);
	std::shared_ptr<cJSON> response = _ticketedSender.send(request.toJSON(), 1000);
	if(response==nullptr){
		return false;
	}
	JHelper::printJsonKeysAndValues(response.get());
	return true;
}
void Port_FiberOpticChannel1::handleIncomingMessage(cJSON* message, bool& dontDelete){
	if(_messageSender==nullptr){
        Log::Error(TAG, "_messageSender was null. You must set it with setMessageSender");
		return;
	}
	bool success = true;
	char* type = JHelper::getString(message, "tpe", success);
	if (!success) {
		return;
	}
	if(strcmp(type, MessageConstants::TYPE_TICKETED_VALUE) == 0){
		Log::Info(TAG, "Got ticketed");
		_ticketedSender.handleTicketedMessage(message, type);
		dontDelete = true;
		return;
	}
	if(strcmp(type, SetVoltageThresholdRequest::TYPE) == 0){
		Log::Info(TAG, "Got set voltage");
		handleSetVoltageThresholdRequest(message);
		return;
	}
	if(strcmp(type, GetVoltageRequest::TYPE) == 0){
		Log::Info(TAG, "Got get voltage");
		handleGetVoltageRequest(message);
		return;
	}
	if(strcmp(type, GreetingRequest::TYPE) == 0){
		Log::Info(TAG, "Got greeting!");
		handleGreetingRequest();
		return;
	}
}
void Port_FiberOpticChannel1::handleSetVoltageThresholdRequest(cJSON* message){
	std::shared_ptr<SetVoltageThresholdRequest> request= SetVoltageThresholdRequest::fromJSON(message);
	double voltage = request->getVoltage();
	uint64_t ticket = request->getTicket();
	_thesholdMonitor.setThresholdVoltage(voltage);
	NonVolatileState::setVoltageThreshold(voltage);
	
	//Log::Info(TAG, "handleSetVoltageThresholdRequest");
	
	//Log::Info(TAG, "ticket from request was %" PRIu64 "", ticket);
	SetVoltageThresholdResponse response(ticket);
	_messageSender->sendMessage(response.toJSON());
	//Log::Info(TAG, "handleSetVoltageThresholdRequest sent response");
}
void Port_FiberOpticChannel1::handleGetVoltageRequest(cJSON* message){
	std::shared_ptr<GetVoltageRequest> request = GetVoltageRequest::fromJSON(message);
	uint64_t ticket = request->getTicket();
	double voltage = _thesholdMonitor.getVoltage();
	GetVoltageResponse response(voltage, ticket);
	_messageSender->sendMessage(response.toJSON());
}
void Port_FiberOpticChannel1::handleGreetingRequest(){
	std::shared_ptr<GreetingRequest> request = GreetingRequest::fromJSON(message);
	uint64_t ticket = request->getTicket();
	
	CleanupBucket cleanupBucket;
	std::shared_ptr<CoreDumpSummaryMessage> coreDumpSummaryMessage 
		= CrashReporter::getCoreDumpSummary(cleanupBucket);
	LastAbortMessage* lastAbortMessage = 	Aborter::getLastAbortReason(cleanupBucket);
	GreetingResponse response(coreDumpSummaryMessage, lastAbortMessage, ticket);
	_messageSender->sendMessage(response.toJSON());
}

