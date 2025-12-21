#include "Port_ControllingMachine.hpp"
#include "Logging/Log.hpp"
#include "System/SafeAbort.hpp"
#include "Timing/Delay.hpp"
#include "JSON/JHelper.hpp"
#include "Tasks/TaskFactory.hpp"
#include "Messaging/MessageConstants.hpp"
#include "Generated/Messages/ConsoleMessage.hpp"
#include "Generated/Messages/RunSystemChecksOnlyMessage.hpp"
#include "Generated/Messages/ShutDownMessage.hpp"
#include "Generated/Messages/StartMessage.hpp"
#include "Generated/Messages/StopMessage.hpp"
#include "Generated/Messages/StateChangedMessage.hpp"
#include "Generated/Messages/PingMessage.hpp"
#include "Generated/Messages/CoreDumpSummaryMessage.hpp"
#include "Generated/Messages/LastAbortMessage.hpp"
#include "Generated/Messages/ClearLoggedErrorsMessage.hpp"
#include "System/CrashReporter.hpp"
#include "Enums/SubsystemIdentifiers.hpp"
#include "Core/CleanupBucket.hpp"
#include "Enums/ErrorType.hpp"
//#include "Generated/Messages/SetVoltageThresholdRequest.hpp"
#include <cstring>
Port_ControllingMachine::Port_ControllingMachine(
	IDuplexChannel& channel, HighSpeedCore& highSpeedCore,
	uint32_t pingTimeoutMilliseconds,
	Port_FirstStageVoltageFeedback& port_FirstStageVoltageFeedback,
	Port_OutputVoltageFeedback& port_OutputVoltageFeedback)
:
_channel(channel),
_highSpeedCore(highSpeedCore),
_ticketedSender(
			[this](cJSON* msg){
				_channel.sendMessage(msg, true);
		}),
_timerSendPing(
	/*uint32_t intervalMs*/ pingTimeoutMilliseconds/2,
	/*Callback callback*/[this](){sendPing();},
	/*bool repeat*/ true
),
_port_FirstStageVoltageFeedback(port_FirstStageVoltageFeedback),
_port_OutputVoltageFeedback(port_OutputVoltageFeedback){
    _channel.setIncomingMessageHandler(this);
	_eventConnectionHighSpeedCoreOnSystemStateChanged = _highSpeedCore.onSystemStateChanged.addHandler(
		[this](SystemState systemState){
			this->handleStateChanged(systemState);
		}
	);
	_eventConnectionHighSpeedCoreOnError = _highSpeedCore.onError.addHandler(
		[this](std::string errorMessage){
			this->handleHighSpeedCoreError(errorMessage);
		}
	);
	_eventConnectionOnOpened = _channel.addOnOpenedHandler([this](const ChannelEventArgs& e){
				handleOnOpened();
	});
	_eventConnectionOnClosed = _channel.addOnClosedHandler([this](const ChannelEventArgs& e){
				handleOnClosed();
	});
	_eventConnectionOnGotGreetingMessageFirstStageVoltageFeedbackModule = _port_FirstStageVoltageFeedback.onGotGreetingMessage.addHandler(
		[this](GreetingMessage* greetingMessage){
			this->handleGotGreetingMessageFromVoltageFeedbackModule(greetingMessage);
		}
	);
	_eventConnectionOnGotGreetingMessageOutputVoltageFeedbackModule = _port_OutputVoltageFeedback.onGotGreetingMessage.addHandler(
		[this](GreetingMessage* greetingMessage){
			this->handleGotGreetingMessageFromVoltageFeedbackModule(greetingMessage);
		}
	);
}
Port_ControllingMachine::~Port_ControllingMachine() noexcept
{
	
}
void Port_ControllingMachine::sendConsoleMessage(const std::string& str, bool isError) {
    ConsoleMessage consoleMessage(isError, str.c_str());   // automatic storage, no `new`
    _channel.sendMessage(consoleMessage.toJSON());
}
void Port_ControllingMachine::sendLiveData(LiveDataMessage liveDataMessage) {
    _channel.sendMessage(liveDataMessage.toJSON());
}
void Port_ControllingMachine::handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity){
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
	if(strcmp(type, RunSystemChecksOnlyMessage::TYPE) == 0){
		handleRunSystemChecksOnlyMessage(message);
		return;
	}
	if(strcmp(type, ShutDownMessage::TYPE) == 0){
		handleShutDownMessage(message);
		return;
	}
	if(strcmp(type, StartMessage::TYPE) == 0){
		handleStartMessage(message);
		return;
	}
	if(strcmp(type, StopMessage::TYPE) == 0){
		handleStopMessage(message);
		return;
	}
	if(strcmp(type, ClearLoggedErrorsMessage::TYPE) == 0){
		handleClearLoggedErrors();
		return;
	}
	if(strcmp(type, PingMessage::TYPE) == 0){
		return;
	}
	Log::Warn(TAG, "Unhandled message type %s", type);
}
void Port_ControllingMachine::handleRunSystemChecksOnlyMessage(cJSON* message){
	TaskFactory::createNonPriorityTask([this](){
			std::shared_ptr<SystemChecksResult> systemChecksResult = _highSpeedCore.runSystemChecksOnly();
		}, 
		"runSystemChecksOnly"
	);
}
void Port_ControllingMachine::handleShutDownMessage(cJSON* message){
	_highSpeedCore.shutDown();
}
void Port_ControllingMachine::handleStartMessage(cJSON* message){
	_highSpeedCore.start();
	
}
void Port_ControllingMachine::handleStopMessage(cJSON* message){
	_highSpeedCore.stop();
}
void Port_ControllingMachine::handleStateChanged(SystemState systemState){		
	StateChangedMessage stateChangedMessage((int32_t)systemState);
	_channel.sendMessage(stateChangedMessage.toJSON());
}
void Port_ControllingMachine::sendPing(){	
	PingMessage pingMessage;
	_channel.sendMessage(pingMessage.toJSON());
}
void Port_ControllingMachine::handleOnOpened(){
	_timerSendPing.start();
	sendErrors();
	sendState();
}
void Port_ControllingMachine::handleOnClosed(){
	_timerSendPing.stop();
}
void Port_ControllingMachine::handleClearLoggedErrors(){
	CrashReporter::clearRecord();
	Aborter::clearLastAbortReason();
	_highSpeedCore.setInError(false);
	_port_FirstStageVoltageFeedback.sendClearLoggedErrors();
	_port_OutputVoltageFeedback.sendClearLoggedErrors();
	 sendConsoleMessage("Cleared errors!", false);
}
void Port_ControllingMachine::sendErrors(){
	CleanupBucket cleanupBucket;
	CoreDumpSummaryMessage* coreDumpSummaryMessage 
		= CrashReporter::getCoreDumpSummary(cleanupBucket);
	LastAbortMessage* lastAbortMessage = 
		Aborter::getLastAbortReason(cleanupBucket);
	sendErrors(
		coreDumpSummaryMessage, 
		lastAbortMessage
	);
	greetVoltageFeedbackModules();
}
void Port_ControllingMachine::sendErrors(
	CoreDumpSummaryMessage* coreDumpSummaryMessage, 
	LastAbortMessage* lastAbortMessage
){
	if(coreDumpSummaryMessage){
		_channel.sendMessage(coreDumpSummaryMessage->toJSON());
	}
	if(lastAbortMessage){
		_channel.sendMessage(lastAbortMessage->toJSON());
	}
}
uint32_t Port_ControllingMachine::greetVoltageFeedbackModules(){
	CleanupBucket cleanupBucket;
	GreetingResponse* greetingResponse = _port_FirstStageVoltageFeedback.greet(
		cleanupBucket);
	int32_t subsystemIdentifierWithError = 0;
	if(
		(greetingResponse!=nullptr)
		&&(greetingResponse->getCoreDumpSummaryMessage()!=nullptr)
		&&(greetingResponse->getLastAbortMessage()!=nullptr)
	){
		subsystemIdentifierWithError = SubsystemIdentifiers::FirstStageVoltageFeedbackModule;
		sendErrors(greetingResponse->getCoreDumpSummaryMessage(),
			greetingResponse->getLastAbortMessage());
	}
	greetingResponse = _port_OutputVoltageFeedback.greet(cleanupBucket);
	if(
		(greetingResponse!=nullptr)
		&&(greetingResponse->getCoreDumpSummaryMessage()!=nullptr)
		&&(greetingResponse->getLastAbortMessage()!=nullptr)
	){
		subsystemIdentifierWithError = SubsystemIdentifiers::OutputVoltageFeedbackModule;
		sendErrors(greetingResponse->getCoreDumpSummaryMessage(),
			greetingResponse->getLastAbortMessage());
	}
	return subsystemIdentifierWithError;
}
void Port_ControllingMachine::handleGotGreetingMessageFromVoltageFeedbackModule(
	GreetingMessage* greetingMessage){
	sendErrors(greetingMessage->getCoreDumpSummaryMessage(),
		greetingMessage->getLastAbortMessage());
}
void Port_ControllingMachine::sendState(){
	SystemState systemState = _highSpeedCore.getActualSystemState();
	StateChangedMessage stateChangedMessage((int32_t)systemState);
	_channel.sendMessage(stateChangedMessage.toJSON());
}
void Port_ControllingMachine::handleHighSpeedCoreError(std::string errorMessage){
	sendConsoleMessage(errorMessage, true);
}

