#include "Port_VoltageFeedbackBase.hpp"
#include "../Logging/Log.hpp"
#include "../System/Aborter.hpp"
#include "../JSON/JHelper.hpp"
#include "../Messaging/MessageConstants.hpp"
#include "../Generated/Messages/SetVoltageThresholdRequest.hpp"
#include "../Generated/Messages/SetVoltageThresholdResponse.hpp"
#include "../Generated/Messages/GetVoltageThresholdRequest.hpp"
#include "../Generated/Messages/GetVoltageThresholdResponse.hpp"
#include "../Generated/Messages/GetVoltageRequest.hpp"
#include "../Generated/Messages/GetVoltageResponse.hpp"
#include "../Generated/Messages/VoltageMessage.hpp"
#include <memory>
#include <cstring>
Port_VoltageFeedbackBase::Port_VoltageFeedbackBase(TOSLINKDuplexChannel* toslinkDuplexChannel)
:
_TOSLINKDuplexChannel(toslinkDuplexChannel),
_ticketedSender(
			[this](cJSON* msg){
				_TOSLINKDuplexChannel->sendMessage(msg, true);
		}){
    _TOSLINKDuplexChannel->setIncomingMessageHandler(this);
	_TOSLINKDuplexChannel->startAsNewNonPriorityTask();
	_messageSender = _TOSLINKDuplexChannel;
}
Port_VoltageFeedbackBase::~Port_VoltageFeedbackBase() noexcept;
{
    delete _TOSLINKDuplexChannel;
	_TOSLINKDuplexChannel = nullptr; 
}
bool Port_VoltageFeedbackBase::setVoltageThreshold(double voltage){
	SetVoltageThresholdRequest request(voltage);
	std::shared_ptr<cJSON> response = _ticketedSender.send(request.toJSON());
	if(response==nullptr){
		return false;
	}
	return true;
}
bool Port_VoltageFeedbackBase::getVoltageThreshold(double& voltage) {
    voltage = -1.0;

    GetVoltageThresholdRequest request;   // NOTE: not request()
    shared_ptr<cJSON> jsonResponse = _ticketedSender.send(request.toJSON());
    if (!jsonResponse) {
        return false;
    }
	std::shared_ptr<GetVoltageThresholdResponse> response = GetVoltageThresholdResponse::fromJSON(jsonResponse.get()));
    voltage = response.get()->getVoltage();
    return true;
}
bool Port_VoltageFeedbackBase::getVoltage(double& voltage) {
    voltage = -1.0;

    GetVoltageRequest request;   // NOTE: not request()
    std::shared_ptr<cJSON> jsonResponse = _ticketedSender.send(request.toJSON());
    if (!jsonResponse) {
        return false;
    }
	std::shared_ptr<GetVoltageResponse> response = GetVoltageResponse::fromJSON(jsonResponse.get()));
    voltage = response.get()->getVoltage();
    return true;
}
void Port_VoltageFeedbackBase::handleIncomingMessage(cJSON* message, bool& dontDelete){
	if(_messageSender==nullptr){
        Log::Error(getTag(), "_messageSender was null. You must set it with setMessageSender");
		return;
	}
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
	if(strcmp(type, VoltageMessage::TYPE) == 0){
		handleVoltageMessage(message);
		return;
	}
}
void Port_VoltageFeedbackBase::handleVoltageMessage(cJSON* message){
	std::shared_ptr<VoltageMessage> voltageMessage = VoltageMessage::fromJSON(message);
	double voltage = voltageMessage.get().getVoltage();
	onGotVoltage.dispatch(voltage);
}
