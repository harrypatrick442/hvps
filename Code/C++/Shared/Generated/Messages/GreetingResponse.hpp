#ifndef GREETINGRESPONSE_HPP
#define GREETINGRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "./CoreDumpSummaryMessage.hpp"
#include "./LastAbortMessage.hpp"
#include "../../JSON/JHelper.hpp"
class GreetingResponse
{
   public:
       static const char* TYPE;
   private:
        std::shared_ptr<CoreDumpSummaryMessage> _coreDumpSummaryMessage;
        std::shared_ptr<LastAbortMessage> _lastAbortMessage;
        uint64_t _ticket;
   public:
        std::shared_ptr<CoreDumpSummaryMessage> getCoreDumpSummaryMessage() noexcept;
        std::shared_ptr<LastAbortMessage> getLastAbortMessage() noexcept;
        uint64_t getTicket() noexcept;
        GreetingResponse(
           std::shared_ptr<CoreDumpSummaryMessage>, 
           std::shared_ptr<LastAbortMessage>, 
           uint64_t ticket) noexcept;
        ~GreetingResponse();
        static std::shared_ptr<GreetingResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GREETINGRESPONSE_HPP
