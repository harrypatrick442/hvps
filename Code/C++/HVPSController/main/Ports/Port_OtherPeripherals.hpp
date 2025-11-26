#pragma once

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IMessageSender.hpp"
#include "Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Core/CleanupBucket.hpp"
#include "Core/SingletonBase.hpp"
#include "Enums/SystemState.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "cJSON/cJSON.h"
#include "Core/Macros.hpp"
#include "Core/Event.hpp"

class Port_OtherPeripherals final:
	public SingletonBase<Port_OtherPeripherals>,
	public IIncomingMessageHandler{
    friend class SingletonBase<Port_OtherPeripherals>;
public:

	inline static constexpr const char* TAG = "Port_OtherPeripherals";
    // Force derived classes to identify themselves
	DISALLOW_COPY_MOVE(Port_OtherPeripherals);

    void handleIncomingMessage(cJSON* message, bool& dontDelete) override;
	bool sendIndicateStateRequest();
	void sendIndicateStateMessage();

protected:
    explicit Port_OtherPeripherals() noexcept;
	virtual ~Port_OtherPeripherals();

protected:
	static inline constexpr int TIMEOUT = 1000;
    IMessageSender*       _messageSender = nullptr;
    TOSLINKDuplexChannel*  _TOSLINKDuplexChannel = nullptr;
    TicketedSender        _ticketedSender;
private:
	void handleSendStateToIndicateMessage();
	void setTarget(cJSON* json, uint32_t target);
};