#pragma once

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IDuplexChannel.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Timing/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Enums/SystemState.hpp"
#include "../ControllerCore/HighSpeedCore.hpp"
#include "Port_FirstStageVoltageFeedback.hpp"
#include "Port_OutputVoltageFeedback.hpp"
#include "Communication/Enums/MessageIntegrity.hpp"
#include "cJSON/cJSON.h"
#include <string>

class Port_ControllingMachine final:
	public SingletonBase<Port_ControllingMachine>,
	public IIncomingMessageHandler {
    friend class SingletonBase<Port_ControllingMachine>;
public:
	static const char* getTag();
    // Force derived classes to identify themselves

    void handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity) override;

	DISALLOW_COPY_MOVE(Port_ControllingMachine);
	
	void sendConsoleMessage(const std::string& str, bool isError = true);
	void sendLiveData(LiveDataMessage liveDataMessage);
	//return errorFree
	uint32_t greetVoltageFeedbackModules();

protected:
    explicit Port_ControllingMachine(
		IDuplexChannel& channel,
		HighSpeedCore& highSpeedCore,
		uint32_t pingTimeoutMilliseconds,
		Port_FirstStageVoltageFeedback& port_FirstStageVoltageFeedback,
		Port_OutputVoltageFeedback& port_OutputVoltageFeedback)noexcept;
	virtual ~Port_ControllingMachine();
    IDuplexChannel&  _channel;
	HighSpeedCore& _highSpeedCore;
    TicketedSender	_ticketedSender;
	Timer _timerSendPing;
	Port_FirstStageVoltageFeedback& _port_FirstStageVoltageFeedback;
	Port_OutputVoltageFeedback& _port_OutputVoltageFeedback;
	
private:
	EventConnection _eventConnectionHighSpeedCoreOnSystemStateChanged;
	EventConnection _eventConnectionHighSpeedCoreOnError;
	EventConnection _eventConnectionOnOpened;
	EventConnection _eventConnectionOnClosed;
	EventConnection _eventConnectionOnGotGreetingMessageFirstStageVoltageFeedbackModule;
	EventConnection _eventConnectionOnGotGreetingMessageOutputVoltageFeedbackModule;
	void handleRunSystemChecksOnlyMessage(cJSON* message);
	void handleShutDownMessage(cJSON* message);
	void handleStartMessage(cJSON* message);
	void handleStopMessage(cJSON* message);
	void handleStateChanged(SystemState systemState);
	void handleClearLoggedErrors();
	void sendPing();
	
	void handleOnOpened();
	void handleOnClosed();
	void sendErrors();
	void sendErrors(
		CoreDumpSummaryMessage* coreDumpSummaryMessage, 
		LastAbortMessage* lastAbortMessage
	);
	void sendState();
	void handleGotGreetingMessageFromVoltageFeedbackModule(GreetingMessage* greetingMessage);
	void handleHighSpeedCoreError(std::string errorMessage);
};