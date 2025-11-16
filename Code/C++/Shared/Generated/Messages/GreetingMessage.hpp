#ifndef GREETINGMESSAGE_HPP
#define GREETINGMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "./CoreDumpSummaryMessage.hpp"
#include "./LastAbortMessage.hpp"
#include "../../JSON/JHelper.hpp"
class GreetingMessage
{
   public:
       static const char* TYPE;
   private:
        std::shared_ptr<CoreDumpSummaryMessage> _coreDumpSummaryMessage;
        std::shared_ptr<LastAbortMessage> _lastAbortMessage;
   public:
        std::shared_ptr<CoreDumpSummaryMessage> getCoreDumpSummaryMessage() noexcept;
        std::shared_ptr<LastAbortMessage> getLastAbortMessage() noexcept;
        GreetingMessage(
           std::shared_ptr<CoreDumpSummaryMessage>, 
           std::shared_ptr<LastAbortMessage>) noexcept;
        ~GreetingMessage();
        static std::shared_ptr<GreetingMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GREETINGMESSAGE_HPP
