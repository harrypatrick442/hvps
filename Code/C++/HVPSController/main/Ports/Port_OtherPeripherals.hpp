#ifndef Port_VoltageFeedbackBase_hpp
#define Port_VoltageFeedbackBase_hpp

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IMessageSender.hpp"
#include "Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Core/CleanupBucket.hpp"
#include "../Enums/SystemState.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "cJSON/cJSON.h"
#include "Core/Macros.hpp"
#include "Core/Event.hpp"

class Port_OtherPeripherals : public IIncomingMessageHandler {
public:

    // Force derived classes to identify themselves
    virtual const char* getTag() const = 0;
	DISALLOW_COPY_MOVE(Port_OtherPeripherals);

    void handleIncomingMessage(cJSON* message, bool& dontDelete) override;
	void sendIndicateStateRequest();
	void sendIndicateStateMessage();

protected:
    explicit Port_OtherPeripherals(TOSLINKDuplexChannel* channel);
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

#endif
