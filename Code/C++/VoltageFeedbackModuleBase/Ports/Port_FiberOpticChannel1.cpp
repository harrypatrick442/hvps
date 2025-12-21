#include "Port_FiberOpticChannel1.hpp"
#include "Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "JSON/JHelper.hpp"
#include "../IO/Inputs.hpp"
#include "Messaging/MessageConstants.hpp"
#include "Generated/Messages/GreetingRequest.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Generated/Messages/ClearLoggedErrorsMessage.hpp"
#include "Tasks/TaskFactory.hpp"
#include "System/CrashReporter.hpp"
#include <cstring>
#include <memory>
#include "Macros/GetFileName.hpp"
const char* Port_FiberOpticChannel1::getTag() {return GET_FILE_NAME;}

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
	TaskFactory::createNonPriorityTask(
		[&](){
			greetControllingMachine();
		}, 
		"greetControllingMachine"
	);
}
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
	LOG_INFO("Handling message");
	if(_messageSender==nullptr){
        LOG_ERROR("_messageSender was null. You must set it with setMessageSender");
		return;
	}
	bool success = true;
	char* type = JHelper::getString(message, "tpe", success);
	if (!success) {
		return;
	}
	LOG_INFO("type: %s", type);
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
	if(strcmp(type, GreetingRequest::TYPE) == 0){
		LOG_INFO("Got greeting!");
		handleGreetingRequest(message);
		return;
	}
	if(strcmp(type, ClearLoggedErrorsMessage::TYPE) == 0){
		LOG_INFO("Got clear logged errors!");
		handleClearLoggedErrorsMessage();
		return;
	}
}
void Port_FiberOpticChannel1::handleSetVoltageThresholdRequest(cJSON* message){
	CleanupBucket cleanupBucket;
	SetVoltageThresholdRequest* request= SetVoltageThresholdRequest::fromJSON(message, cleanupBucket);
	float voltage = request->getVoltage();
	uint64_t ticket = request->getTicket();
	_thesholdMonitor.setThresholdVoltage(voltage);
	
	//LOG_INFO("handleSetVoltageThresholdRequest");
	
	//LOG_INFO("ticket from request was %" PRIu64 "", ticket);
	SetVoltageThresholdResponse response(ticket);
	_messageSender->sendMessage(response.toJSON());
	//LOG_INFO("handleSetVoltageThresholdRequest sent response");
}
void Port_FiberOpticChannel1::handleGetVoltageRequest(cJSON* message){
	CleanupBucket cleanupBucket;
	GetVoltageRequest* request = GetVoltageRequest::fromJSON(message, cleanupBucket);
	uint64_t ticket = request->getTicket();
	float voltage = _thesholdMonitor.getVoltage();
	GetVoltageResponse response(voltage, ticket);
	_messageSender->sendMessage(response.toJSON());
}
void Port_FiberOpticChannel1::handleGreetingRequest(cJSON* message){
	CleanupBucket cleanupBucket;
	GreetingRequest* request = GreetingRequest::fromJSON(message, cleanupBucket);
	uint64_t ticket = request->getTicket();
	LastAbortMessage* lastAbortMessage 
		= Aborter::getLastAbortReason(cleanupBucket);
	CoreDumpSummaryMessage* coreDumpSummaryMessage 
		= CrashReporter::getCoreDumpSummary(cleanupBucket);
	GreetingResponse response(coreDumpSummaryMessage, lastAbortMessage, ticket);
	_messageSender->sendMessage(response.toJSON());
}
void Port_FiberOpticChannel1::greetControllingMachine(){
	CleanupBucket cleanupBucket;
	CoreDumpSummaryMessage* coreDumpSummaryMessage 
		= CrashReporter::getCoreDumpSummary(cleanupBucket);
	LastAbortMessage* lastAbortMessage = Aborter::getLastAbortReason(cleanupBucket);
	GreetingMessage message(coreDumpSummaryMessage, lastAbortMessage);
	_messageSender->sendMessage(message.toJSON());
}
void Port_FiberOpticChannel1::handleClearLoggedErrorsMessage(){
	CrashReporter::clearRecord();
	Aborter::clearLastAbortReason();
	LOG_INFO("Cleared logged errors!");
}

