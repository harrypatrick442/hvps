#ifndef STOPMESSAGE_HPP
#define STOPMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class StopMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        StopMessage(
) noexcept;
        ~StopMessage();
        static StopMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STOPMESSAGE_HPP
