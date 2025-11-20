#ifndef PINGMESSAGE_HPP
#define PINGMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class PingMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        PingMessage(
) noexcept;
        ~PingMessage();
        static PingMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //PINGMESSAGE_HPP
