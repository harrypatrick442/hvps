#ifndef HASBUSMESSAGE_HPP
#define HASBUSMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class HasBusMessage
{
   public:
       static const char* TYPE;
   private:
        int32_t _target;
   public:
        int32_t getTarget()const noexcept;
        HasBusMessage(
           int32_t target) noexcept;
        ~HasBusMessage();
        static HasBusMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HASBUSMESSAGE_HPP
