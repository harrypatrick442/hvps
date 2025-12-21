#ifndef GREETINGMESSAGE_HPP
#define GREETINGMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "CoreDumpSummaryMessage.hpp"
#include "System/SafeAbort.hpp"
#include "LastAbortMessage.hpp"
#include "../../JSON/JHelper.hpp"
class GreetingMessage
{
   public:
       static const char* TYPE;
   private:
        CoreDumpSummaryMessage* _coreDumpSummaryMessage;
        LastAbortMessage* _lastAbortMessage;
   public:
        CoreDumpSummaryMessage* getCoreDumpSummaryMessage()const noexcept;
        LastAbortMessage* getLastAbortMessage()const noexcept;
        GreetingMessage(
           CoreDumpSummaryMessage* coreDumpSummaryMessage, 
           LastAbortMessage* lastAbortMessage) noexcept;
        ~GreetingMessage();
        static GreetingMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GREETINGMESSAGE_HPP
