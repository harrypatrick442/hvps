#ifndef STARTMESSAGE_HPP
#define STARTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class StartMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        StartMessage(
) noexcept;
        ~StartMessage();
        static StartMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STARTMESSAGE_HPP
