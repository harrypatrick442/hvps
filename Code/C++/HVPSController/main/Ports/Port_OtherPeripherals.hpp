#pragma once

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IMessageSender.hpp"
#include "Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Core/CleanupBucket.hpp"
#include "Core/SingletonBase.hpp"
#include "../ControllerCore/HighSpeedCore.hpp"
#include "Enums/SystemState.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "Core/Macros.hpp"
#include "Core/Event.hpp"
#include "Communication/Enums/MessageIntegrity.hpp"
class Port_OtherPeripherals final:
	public SingletonBase<Port_OtherPeripherals>,
	public IIncomingMessageHandler{
    friend class SingletonBase<Port_OtherPeripherals>;
public:

	inline static constexpr const char* TAG = "Port_OtherPeripherals";
    // Force derived classes to identify themselves
	DISALLOW_COPY_MOVE(Port_OtherPeripherals);

    void handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity) override;
	bool sendIndicateStateRequest();
	void sendIndicateStateMessage();

protected:
    explicit Port_OtherPeripherals(HighSpeedCore& highSpeedCore) noexcept;
	virtual ~Port_OtherPeripherals();

private:
	static inline constexpr int TIMEOUT = 1000;
	HighSpeedCore& _highSpeedCore;
    IMessageSender*       _messageSender = nullptr;
    TOSLINKDuplexChannel*  _TOSLINKDuplexChannel = nullptr;
    TicketedSender        _ticketedSender;
	EventConnection _eventConnectionHighSpeedCoreOnSystemStateChanged;
private:
	void handleSendStateToIndicateMessage();
	void setTarget(cJSON* json, uint32_t target);
};