#ifndef SHUTDOWNMESSAGE_HPP
#define SHUTDOWNMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class ShutDownMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        ShutDownMessage(
) noexcept;
        ~ShutDownMessage();
        static ShutDownMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SHUTDOWNMESSAGE_HPP
