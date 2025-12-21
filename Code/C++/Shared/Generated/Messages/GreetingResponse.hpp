#ifndef GREETINGRESPONSE_HPP
#define GREETINGRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "CoreDumpSummaryMessage.hpp"
#include "System/SafeAbort.hpp"
#include "LastAbortMessage.hpp"
#include "../../JSON/JHelper.hpp"
class GreetingResponse
{
   public:
       static const char* TYPE;
   private:
        CoreDumpSummaryMessage* _coreDumpSummaryMessage;
        LastAbortMessage* _lastAbortMessage;
        uint64_t _ticket;
   public:
        CoreDumpSummaryMessage* getCoreDumpSummaryMessage()const noexcept;
        LastAbortMessage* getLastAbortMessage()const noexcept;
        uint64_t getTicket()const noexcept;
        GreetingResponse(
           CoreDumpSummaryMessage* coreDumpSummaryMessage, 
           LastAbortMessage* lastAbortMessage, 
           uint64_t ticket) noexcept;
        ~GreetingResponse();
        static GreetingResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GREETINGRESPONSE_HPP
