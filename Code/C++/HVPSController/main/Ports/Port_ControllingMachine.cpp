#include "Port_ControllingMachine.hpp"
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include "../JSON/JHelper.hpp"
#include "../Messaging/MessageConstants.hpp"
#include "../Generated/Messages/ConsoleMessage.hpp"
#include "../States/SystemState.hpp"
#include "../States/SystemState.hpp"
//#include "../Generated/Messages/SetVoltageThresholdRequest.hpp"
#include <cstring>
Port_ControllingMachine::Port_ControllingMachine(IChannel& channel)
:
_channel(channel),
_ticketedSender(
			[this](cJSON* msg){
				_channel.sendMessage(msg, true);
		}){
    _channel.setIncomingMessageHandler(this);
	State.getInstance().onSystemStateChanged.addHandler(handleStateChanged);
}
Port_ControllingMachine::~Port_ControllingMachine() noexcept
{
	
}
void Port_ControllingMachine::sendConsoleMessage(const std::string& str) {
    ConsoleMessage consoleMessage(str.c_str(), false);   // automatic storage, no `new`
    _channel.sendMessage(consoleMessage.toJSON());
}
void Port_ControllingMachine::sendHVPSLiveData(HVPSLiveDataMessage hvpsLiveDataMessage) {
    _channel.sendMessage(hvpsLiveDataMessage.toJSON());
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
	if(strcmp(type, HVPSRunSystemChecksOnlyMessage::TYPE) == 0){
		handleRunSystemChecksOnlyMessage(message);
		return;
	}
	if(strcmp(type, HVPSShutDownMessage::TYPE) == 0){
		handleShutDownMessage(message);
		return;
	}
	if(strcmp(type, HVPSStartMessage::TYPE) == 0){
		handleStartMessage(message);
		return;
	}
	if(strcmp(type, HVPSStopMessage::TYPE) == 0){
		handleStopMessage(message);
		return;
	}
}
void Port_ControllingMachine::handleRunSystemChecksOnlyMessage(cJSON* message){
	shared_ptr<SystemChecksResult> systemChecksResult = HighSpeedCore::runSystemChecksOnly();
}
void Port_ControllingMachine::handleShutDownMessage(cJSON* message){
	HighSpeedCore::shutDown();
}
void Port_ControllingMachine::handleStartMessage(cJSON* message){
	HighSpeedCore::start();
	
}
void Port_ControllingMachine::handleStopMessage(cJSON* message){
	HighSpeedCore::stop();
}
void Port_ControllingMachine::handleStateChanged(SystemState systemState){		
	HVPSStateChangedMessage hvpsStateChangedMessage((int32_t)systemState);
	_channel.sendMessage(hvpsStateChangedMessage);
}

