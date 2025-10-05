#ifndef TicketedSenderHandle_HPP
#define TicketedSenderHandle_HPP

#include "../cJSON/cJSON.h"
#include "../Core/Latch.hpp"
#include "../Core/CancellationToken.hpp"
class TicketedSenderHandle final
{
    //template <class> friend class UsageGuard; // <-- gives the guard access
	private:
		uint64_t _timeoutAtThresholdMilliseconds;
		cJSON* _responseMessage;//volatile or atomic not needed because of write and read order with latch.
		Latch _latchWait;
		std::mutex _mutex;
	public:
		TicketedSenderHandle(uint64_t timeoutAtThresholdMilliseconds);
		bool complete(cJSON* responseMessage);
		void fail();
		cJSON* wait();
		cJSON* wait(CancellationToken* cancellationToken);
		bool checkTimedOut(uint64_t millisecondsNow);
};
#endif //TicketedSenderHandle_HPP
