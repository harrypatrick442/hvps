#ifndef TicketedSender_HPP
#define TicketedSender_HPP

#include "../cJSON/cJSON.h"
#include "../Core/CancellationToken.hpp"
#include "../Core/Timer.hpp"
#include "TicketedSenderHandle.hpp"
#include <map>
#include <unordered_set>
using CallbackSend = std::function<void(cJSON* json)>;
class TicketedSender
{
   public:
       static const char* TAG;
	private:
        static Timer _timerCheckTimedOutAllInstances;
        static std::unordered_set<TicketedSender*> _instances;
		static std::mutex _mutexInstances;
		bool _disposed;
		std::mutex _mutex;
		std::map<uint64_t, TicketedSenderHandle*> _mapTicketToHandle;
		CallbackSend _callbackSend;
   public:
        TicketedSender(CallbackSend callbackSend);
        ~TicketedSender();
        cJSON* send(cJSON* request, uint64_t timeoutMilliseconds, CancellationToken* cancellationToken = nullptr);
		bool handleTicketedMessage(cJSON* message);
		bool handleTicketedMessage(cJSON* message, char* type);
		void removeHandle(TicketedSenderHandle* handle, uint64_t ticket);
	private:
		static void checkTimedOutOnAllInstances();
		void checkTimedOuts(uint64_t millisecondsNow);
		void markDisposedAndCleanupHandles();
		void removeFromInstances(TicketedSender* ticketedSender);
};
#endif //TicketedSender_HPP
