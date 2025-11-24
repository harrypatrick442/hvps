#ifndef INDICATESTATEMESSAGE_HPP
#define INDICATESTATEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class IndicateStateMessage
{
   public:
       static const char* TYPE;
   private:
        int32_t _systemState;
   public:
        int32_t getSystemState()const noexcept;
        IndicateStateMessage(
           int32_t systemState) noexcept;
        ~IndicateStateMessage();
        static IndicateStateMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //INDICATESTATEMESSAGE_HPP
