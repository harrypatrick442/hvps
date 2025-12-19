#ifndef Port_VoltageFeedbackBase_hpp
#define Port_VoltageFeedbackBase_hpp

#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IMessageSender.hpp"
#include "Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "Generated/Messages/GreetingResponse.hpp"
#include "Generated/Messages/GreetingMessage.hpp"
#include "Core/CleanupBucket.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "cJSON/cJSON.h"
#include "Core/Macros.hpp"
#include "Core/Event.hpp"
#include "Communication/Enums/MessageIntegrity.hpp"

class Port_VoltageFeedbackBase : public IIncomingMessageHandler {
public:

    // Force derived classes to identify themselves
    virtual const char* getTag() const = 0;
    Event<float> onGotVoltage;
    Event<GreetingMessage*> onGotGreetingMessage;
	DISALLOW_COPY_MOVE(Port_VoltageFeedbackBase);

    void handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity) override;
	bool setVoltageThreshold(float voltage);
    bool getVoltageThreshold(float& voltage);
    bool getVoltage(float& voltage);
	bool setForceThresholdReachedFeedback(bool force);
	void handleVoltageMessage(cJSON* message);
	void sendClearLoggedErrors();
	GreetingResponse* greet(CleanupBucket& cleanupBucket);

protected:
    explicit Port_VoltageFeedbackBase(TOSLINKDuplexChannel* channel);
	virtual ~Port_VoltageFeedbackBase();

protected:
	static inline constexpr int TIMEOUT = 1000;
    IMessageSender*       _messageSender = nullptr;
    TOSLINKDuplexChannel*  _TOSLINKDuplexChannel = nullptr;
    TicketedSender        _ticketedSender;
private:
	void handleGreetingMessage(cJSON* message);

};

#endif
