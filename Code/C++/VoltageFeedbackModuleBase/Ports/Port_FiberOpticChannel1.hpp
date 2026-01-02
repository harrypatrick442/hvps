#ifndef Port_FiberOpticChannel1_hpp
#define Port_FiberOpticChannel1_hpp
#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IMessageSender.hpp"
#include "../ThresholdMonitor.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "../IO/FiberOpticDuplexChannel_1.hpp"
#include "Generated/Messages/SetVoltageThresholdRequest.hpp"
#include "Generated/Messages/SetVoltageThresholdResponse.hpp"
#include "Generated/Messages/GetVoltageRequest.hpp"
#include "Generated/Messages/GetVoltageResponse.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "cJSON/cJSON.h"
#include "Communication/Enums/MessageIntegrity.hpp"
class Port_FiberOpticChannel1  final:
	public SingletonBase<Port_FiberOpticChannel1>,
	public IIncomingMessageHandler
{
    friend class SingletonBase<Port_FiberOpticChannel1>;
	protected:
		static inline constexpr int TIMEOUT = 1000;
    private:
		ThresholdMonitor& _thesholdMonitor;
        IMessageSender* _messageSender;
		FiberOpticDuplexChannel_1 _fiberOpticChannel_1;
		TicketedSender _ticketedSender;
    public :
		static const char* getTag();
		void handleIncomingMessage(cJSON* message, bool& dontDelete,
			MessageIntegrity messageIntegrity) override;
		DISALLOW_COPY_MOVE(Port_FiberOpticChannel1);
		
		bool setVoltageThreshold(float voltage);
		void sendVoltage(float voltage, uint16_t rawVoltage);
		
	protected:
		explicit Port_FiberOpticChannel1(
			ThresholdMonitor& thesholdMonitor
	)noexcept;
	
    private:
		void handleSetVoltageThresholdRequest(cJSON* message);
		void handleGetVoltageRequest(cJSON* message);
		void handleGreetingRequest(cJSON* message);
		void handleClearLoggedErrorsMessage();
		void handleSetForceVoltageThresholdReachedFeedbackRequest(cJSON* message);
		void greetControllingMachine();
};

#endif // Port_FiberOpticChannel1_hpp