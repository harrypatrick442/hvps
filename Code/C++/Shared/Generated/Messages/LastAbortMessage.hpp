#ifndef LASTABORTMESSAGE_HPP
#define LASTABORTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class LastAbortMessage
{
   public:
       static const char* TYPE;
   private:
        const char* _reason;
   public:
        const char* getReason() noexcept;
        LastAbortMessage(
           const char* reason) noexcept;
        ~LastAbortMessage();
        static std::shared_ptr<LastAbortMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //LASTABORTMESSAGE_HPP
