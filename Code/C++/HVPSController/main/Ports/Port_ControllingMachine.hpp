#pragma once

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IChannel.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "Generated/Messages/LiveDataMessage.hpp"
#include "Core/Timer.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "../Enums/SystemState.hpp"
#include "../ControllerCore/HighSpeedCore.hpp"
#include "cJSON/cJSON.h"
#include <string>

class Port_ControllingMachine final:
	public SingletonBase<Port_ControllingMachine>,
	public IIncomingMessageHandler {
    friend class SingletonBase<Port_ControllingMachine>;
public:
	static inline constexpr const char* TAG = "Port_ControllingMachine";
    // Force derived classes to identify themselves

    void handleIncomingMessage(cJSON* message, bool& dontDelete) override;

	DISALLOW_COPY_MOVE(Port_ControllingMachine);
	
	void sendConsoleMessage(const std::string& str);
	void sendLiveData(LiveDataMessage liveDataMessage);

protected:
    explicit Port_ControllingMachine(
		IChannel& channel,
		HighSpeedCore& highSpeedCore,
		uint32_t pingTimeoutMilliseconds)noexcept;
	virtual ~Port_ControllingMachine();
    IChannel&  _channel;
	HighSpeedCore& _highSpeedCore;
    TicketedSender	_ticketedSender;
	Timer _timerSendPing;
private:
	EventConnection _eventConnectionOnOpened;
	EventConnection _eventConnectionOnClosed;
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
};