#ifndef RUNSYSTEMCHECKSONLYMESSAGE_HPP
#define RUNSYSTEMCHECKSONLYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class RunSystemChecksOnlyMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        RunSystemChecksOnlyMessage(
) noexcept;
        ~RunSystemChecksOnlyMessage();
        static RunSystemChecksOnlyMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //RUNSYSTEMCHECKSONLYMESSAGE_HPP
