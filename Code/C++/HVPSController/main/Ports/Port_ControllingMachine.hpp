#pragma once

#include "../Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "../Communication/Interfaces/IChannel.hpp"
#include "../Ticketing/TicketedSender.hpp"
#include "../Generated/Messages/HVPSLiveDataMessage.hpp"
#include "../Core/SingletonBase.hpp"
#include "../Core/Macros.hpp"
#include "../cJSON/cJSON.h"
#include <string>

class Port_ControllingMachine final:
	public SingletonBase<Port_ControllingMachine>,
	public IIncomingMessageHandler {
public:
	static inline constexpr const char* TAG = "Port_ControllingMachine";
    // Force derived classes to identify themselves

    void handleIncomingMessage(cJSON* message, bool& dontDelete) override;

	DISALLOW_COPY_MOVE(Port_ControllingMachine);
	
	void sendConsoleMessage(const std::string& str);
	void sendHVPSLiveData(HVPSLiveDataMessage hvpsLiveDataMessage);

protected:
    explicit Port_ControllingMachine(IChannel& channel)noexcept;
	virtual ~Port_ControllingMachine();

protected:
    IChannel&  _channel;
    TicketedSender	_ticketedSender;
};