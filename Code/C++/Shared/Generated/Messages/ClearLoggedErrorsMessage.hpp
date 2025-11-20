#ifndef CLEARLOGGEDERRORSMESSAGE_HPP
#define CLEARLOGGEDERRORSMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class ClearLoggedErrorsMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        ClearLoggedErrorsMessage(
) noexcept;
        ~ClearLoggedErrorsMessage();
        static ClearLoggedErrorsMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //CLEARLOGGEDERRORSMESSAGE_HPP
