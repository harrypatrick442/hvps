#pragma once

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IDuplexChannel.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Timing/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "Enums/SystemState.hpp"
#include "../ControllerCore/HighSpeedCore.hpp"
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
	
	bool getIsOpen();
protected:
    explicit Port_ControllingMachine(
		IDuplexChannel& channel,
		HighSpeedCore& highSpeedCore,
		uint32_t sendPingIntervalMilliseconds,
		uint32_t pingTimeoutMilliseconds)noexcept;
	virtual ~Port_ControllingMachine();
private:
	
public:
	Event<> onOpened;
	Event<> onClosed;
private:
    IDuplexChannel&  _channel;
	HighSpeedCore& _highSpeedCore;
    TicketedSender	_ticketedSender;
	Timer _timerSendPing;
	Timer _timerCheckReceivedPing;
	std::atomic<bool> _isOpen;
	std::atomic<bool> _receivedPing;
	EventConnection _eventConnectionHighSpeedCoreOnSystemStateChanged;
	EventConnection _eventConnectionHighSpeedCoreOnError;
	EventConnection _eventConnectionHighSpeedCoreOnMessage;
	EventConnection _eventConnectionHighSpeedCoreOnWarning;
	EventConnection _eventConnectionOnOpened;
	EventConnection _eventConnectionOnClosed;
private:
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
	void handleHighSpeedCoreError(std::string errorMessage);
	void handleHighSpeedCoreMessage(std::string message);
	void handleHighSpeedCoreWarning(std::string message);
	void dispatchOnOpened();
	void dispatchOnClosed();
	void checkReceivedPing();
	void handleMayHaveLostComs();
};