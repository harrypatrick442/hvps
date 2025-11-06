#include "Port_ControllingMachine.hpp"
#include "Logging/Log.hpp"
#include "System/Aborter.hpp"
#include "Timing/Delay.hpp"
#include "JSON/JHelper.hpp"
#include "Messaging/MessageConstants.hpp"
#include "Generated/Messages/ConsoleMessage.hpp"
#include "Generated/Messages/RunSystemChecksOnlyMessage.hpp"
#include "Generated/Messages/ShutDownMessage.hpp"
#include "Generated/Messages/StartMessage.hpp"
#include "Generated/Messages/StopMessage.hpp"
#include "Generated/Messages/StateChangedMessage.hpp"
#include "Generated/Messages/PingMessage.hpp"
#include "Generated/Messages/ErrorMessage.hpp"
#include "System/CrashReporter.hpp"
#include "System/CrashRecord.hpp"
#include "Core/CleanupBucket.hpp"
#include "Enums/ErrorType.hpp"
//#include "Generated/Messages/SetVoltageThresholdRequest.hpp"
#include <cstring>
Port_ControllingMachine::Port_ControllingMachine(
	IChannel& channel, HighSpeedCore& highSpeedCore,
	uint32_t pingTimeoutMilliseconds)
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
){
    _channel.setIncomingMessageHandler(this);
	_highSpeedCore.onSystemStateChanged.addHandler(
		[this](SystemState systemState){
			this->handleStateChanged(systemState);
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
void Port_ControllingMachine::sendConsoleMessage(const std::string& str) {
    ConsoleMessage consoleMessage(false, str.c_str());   // automatic storage, no `new`
    _channel.sendMessage(consoleMessage.toJSON());
}
void Port_ControllingMachine::sendLiveData(LiveDataMessage liveDataMessage) {
    _channel.sendMessage(liveDataMessage.toJSON());
}
void Port_ControllingMachine::handleIncomingMessage(cJSON* message, bool& dontDelete){
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
}
void Port_ControllingMachine::handleRunSystemChecksOnlyMessage(cJSON* message){
	std::shared_ptr<SystemChecksResult> systemChecksResult = _highSpeedCore.runSystemChecksOnly();
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
	StateChangedMessage hvpsStateChangedMessage((int32_t)systemState);
	_channel.sendMessage(hvpsStateChangedMessage.toJSON());
}
void Port_ControllingMachine::sendPing(){	
	PingMessage pingMessage;
	_channel.sendMessage(pingMessage.toJSON());
}
void Port_ControllingMachine::handleOnOpened(){
	Log::Info(TAG, "Opened");
	_timerSendPing.start();
	sendErrors();
}
void Port_ControllingMachine::handleOnClosed(){
	Log::Info(TAG, "Closed");
	_timerSendPing.stop();
}
void Port_ControllingMachine::sendErrors(){
	CrashRecord crashRecord;
	CrashReporter::getRecordAndPrint();
	CrashReporter::causePanicOnPurpose();
	return;
	if(CrashReporter::getRecord(crashRecord)){
		
		CleanupBucket cleanupBucket;
		size_t crashRecordLength;
		const char* crashJSON = crashRecord.toJSONString(cleanupBucket, crashRecordLength);
		ErrorMessage errorMessage(
			(int32_t)ErrorType::Panic, 
			crashJSON
		);
		Log::Info(TAG, "crash json is: ");
		Log::Info(TAG, crashJSON);
		
		CrashReporter::causePanicOnPurpose();
		while(true){
			Delay::ms(10000);
		}
		_channel.sendMessage(errorMessage.toJSON());
		CrashReporter::clearRecord();
		Log::Info(TAG, "sent reason");
		return;
	}
	else{
		
		Delay::ms(10000);
		CrashReporter::causePanicOnPurpose();
	}
}

