#ifndef Port_FiberOpticChannel1_hpp
#define Port_FiberOpticChannel1_hpp
#include "Communication/Interfaces/IIncomingMessageHandler.hpp"
#include "Communication/Interfaces/IMessageSender.hpp"
#include "Core/SingletonBase.hpp"
#include "Core/Macros.hpp"
#include "../IO/FiberOpticDuplexChannel_1.hpp"
#include "Generated/Messages/SetVoltageThresholdRequest.hpp"
#include "Generated/Messages/SetVoltageThresholdResponse.hpp"
#include "Generated/Messages/GetVoltageRequest.hpp"
#include "Generated/Messages/GetVoltageResponse.hpp"
#include "Ticketing/TicketedSender.hpp"
#include "Interfaces/ISystemStateIndicator.hpp"
#include "System/SubsystemIdentifier.hpp"
#include "Communication/Enums/MessageIntegrity.hpp"
#include "cJSON/cJSON.h"
class Port_FiberOpticChannel1  final:
	public SingletonBase<Port_FiberOpticChannel1>,
	public IIncomingMessageHandler
{
    friend class SingletonBase<Port_FiberOpticChannel1>;
    public :
		static const char* getTag();
		void handleIncomingMessage(cJSON* message, bool& dontDelete, MessageIntegrity messageIntegrity) override;
		DISALLOW_COPY_MOVE(Port_FiberOpticChannel1);
		
	protected:
		explicit Port_FiberOpticChannel1(
		uint32_t systemIdentifier,
		ISystemStateIndicator& systemStateIndicator
	)noexcept;
		
    private:
		uint32_t _subsystemIdentifier;
		ISystemStateIndicator& _systemStateIndicator;
        IMessageSender* _messageSender;
		FiberOpticDuplexChannel_1 _fiberOpticChannel_1;
		TicketedSender _ticketedSender;
		
		void handleIndicateStateRequest(cJSON* message);
		void handleIndicateStateMessage(cJSON* message);
		void sendSendStateToIndicate();
};

#endif // Port_FiberOpticChannel1_hpp