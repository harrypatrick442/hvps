#ifndef RANSYSTEMCHECKSMESSAGE_HPP
#define RANSYSTEMCHECKSMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class RanSystemChecksMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        RanSystemChecksMessage(
) noexcept;
        ~RanSystemChecksMessage();
        static RanSystemChecksMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //RANSYSTEMCHECKSMESSAGE_HPP
