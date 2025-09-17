#ifndef Port_VoltageFeedbackBase_hpp
#define Port_VoltageFeedbackBase_hpp

#include "../Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "../Communication/Interfaces/IMessageSender.hpp"
#include "../Communication/FiberOptic/TOSLINKDuplexChannel.hpp"
#include "../Enums/SystemState.hpp"
#include "../Ticketing/TicketedSender.hpp"
#include "../cJSON/cJSON.h"
#include "../Core/Macros.hpp"

class Port_VoltageFeedbackBase : public IIncomingMessageHandler {
public:

    // Force derived classes to identify themselves
    virtual const char* getTag() const = 0;
    Event<double> onGotVoltage;
	
	DISALLOW_COPY_MOVE(Port_VoltageFeedbackBase);

    void handleIncomingMessage(cJSON* message, bool& dontDelete) override;
	bool setVoltageThreshold(double voltage);
    bool getVoltageThreshold(double& voltage);
    bool getVoltage(double& voltage);
	void handleVoltageMessage(double voltage);

protected:
    explicit Port_VoltageFeedbackBase(TOSLINKDuplexChannel* channel);
	virtual ~Port_VoltageFeedbackBase();

protected:
    IMessageSender*       _messageSender = nullptr;
    TOSLINKDuplexChannel*  _TOSLINKDuplexChannel = nullptr;
    TicketedSender        _ticketedSender;
};

#endif
