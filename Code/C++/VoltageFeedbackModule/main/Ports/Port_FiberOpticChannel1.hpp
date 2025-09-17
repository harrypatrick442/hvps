#ifndef Port_FiberOpticChannel1_hpp
#define Port_FiberOpticChannel1_hpp
#include "../Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "../Communication/Interfaces/IMessageSender.hpp"
#include "../State.hpp"
#include "../IO/FiberOpticDuplexChannel_1.hpp"
#include "../Generated/Messages/SetVoltageThresholdRequest.hpp"
#include "../Generated/Messages/SetVoltageThresholdResponse.hpp"
#include "../Generated/Messages/GetVoltageRequest.hpp"
#include "../Generated/Messages/GetVoltageResponse.hpp"
#include "../Ticketing/TicketedSender.hpp"
#include "../cJSON/cJSON.h"
class Port_FiberOpticChannel1 : public IIncomingMessageHandler{
    public :
		static const char* TAG;
        static Port_FiberOpticChannel1& initialize(State* state);
        void handleIncomingMessage(cJSON* message, bool& dontDelete) override;
		bool setVoltageThreshold(double voltage);
    private:
        static Port_FiberOpticChannel1* _instance;
        IMessageSender* _messageSender;
		State* _state; 
		FiberOpticDuplexChannel_1 _fiberOpticChannel_1;
		TicketedSender _ticketedSender;
		
        Port_FiberOpticChannel1(State* state);
		void handleSetVoltageThresholdRequest(SetVoltageThresholdRequest* request);
		void handleGetVoltageRequest(GetVoltageRequest* request);
};

#endif // Port_FiberOpticChannel1_hpp