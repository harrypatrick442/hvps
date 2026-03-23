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
#include "Macros/GetFileName.hpp"
const char* Port_ControllingMachine::getTag() {return GET_FILE_NAME;}
Port_ControllingMachine::Port_ControllingMachine(
	IDuplexChannel& channel, 
	HighSpeedCore& highSpeedCore,
	uint32_t sendPingIntervalMilliseconds,
	uint32_t pingTimeoutMilliseconds)
:
_channel(channel),
_highSpeedCore(highSpeedCore),
_ticketedSender(
	[this](cJSON* msg){
		_channel.sendMessage(msg, true);
}),
_timerSendPing(
	/*uint32_t intervalMs*/ sendPingIntervalMilliseconds,
	/*Callback callback*/[this](){sendPing();},
	/*bool repeat*/ true
),
_timerCheckReceivedPing(
	pingTimeoutMilliseconds,
	[this](){checkReceivedPing();},
	true
),
_isOpen(false),
_receivedPing(false)
{
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
	_eventConnectionHighSpeedCoreOnMessage = _highSpeedCore.onMessage.addHandler(
		[this](std::string message){
			this->handleHighSpeedCoreMessage(message);
		}
	);
	_eventConnectionHighSpeedCoreOnWarning = _highSpeedCore.onWarning.addHandler(
		[this](std::string message){
			this->handleHighSpeedCoreWarning(message);
		}
	);
	_eventConnectionOnOpened = _channel.addOnOpenedHandler([this](const ChannelEventArgs& e){
				handleOnOpened();
	});
	_eventConnectionOnClosed = _channel.addOnClosedHandler([this](const ChannelEventArgs& e){
				handleOnClosed();
	});
}
Port_ControllingMachine::~Port_ControllingMachine() noexcept
{
	
}
bool Port_ControllingMachine::getIsOpen(){
	return _isOpen.load(std::memory_order_relaxed);
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
	_receivedPing.store(true, std::memory_order_relaxed);
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
	LOG_WARN("Unhandled message type %s", type);
}
void Port_ControllingMachine::handleRunSystemChecksOnlyMessage(cJSON* message){
	TaskFactory::createNonPriorityTask([this](){
			_highSpeedCore.runSystemChecksOnly();
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
	_receivedPing.store(true, std::memory_order_relaxed);
	_timerCheckReceivedPing.start();
	sendErrors();
	sendState();
	_isOpen.store(true, std::memory_order_relaxed);
	dispatchOnOpened();
}
void Port_ControllingMachine::handleOnClosed(){
	_highSpeedCore.stop();
	_timerSendPing.stop();
	_timerCheckReceivedPing.stop();
	_isOpen.store(false, std::memory_order_relaxed);
	dispatchOnClosed();
}
void Port_ControllingMachine::handleClearLoggedErrors(){
	CrashReporter::clearRecord();
	Aborter::clearLastAbortReason();
	_highSpeedCore.setInError(false);
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
void Port_ControllingMachine::sendState(){
	SystemState systemState = _highSpeedCore.getActualSystemState();
	StateChangedMessage stateChangedMessage((int32_t)systemState);
	_channel.sendMessage(stateChangedMessage.toJSON());
}
void Port_ControllingMachine::handleHighSpeedCoreError(std::string errorMessage){
	sendConsoleMessage(errorMessage, true);
}
void Port_ControllingMachine::handleHighSpeedCoreMessage(std::string message){
	sendConsoleMessage(message, false);
}
void Port_ControllingMachine::handleHighSpeedCoreWarning(std::string message){
	sendConsoleMessage(std::string("WARNING: ")+message, true);
}
void Port_ControllingMachine::dispatchOnOpened(){
		onOpened.dispatch();
}
void Port_ControllingMachine::dispatchOnClosed(){ 
		onClosed.dispatch();
}
void Port_ControllingMachine::checkReceivedPing(){ 
	bool received = _receivedPing.exchange(false, std::memory_order_relaxed);
	if(!received){
		handleMayHaveLostComs();
	}
}
void Port_ControllingMachine::handleMayHaveLostComs(){ 
	//TAKE NO CHANCES, SHUT DOWN IF LIVE.
	_highSpeedCore.stop();
	LOG_WARN("MAY HAVE LOST CONNECTION TO CONTROLLING MACHINE");
}

