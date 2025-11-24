#ifndef INDICATESTATEREQUEST_HPP
#define INDICATESTATEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class IndicateStateRequest
{
   public:
       static const char* TYPE;
   private:
        int32_t _systemState;
        uint64_t _ticket;
   public:
        int32_t getSystemState()const noexcept;
        uint64_t getTicket()const noexcept;
        IndicateStateRequest(
           int32_t systemState, 
           uint64_t ticket = 0) noexcept;
        ~IndicateStateRequest();
        static IndicateStateRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //INDICATESTATEREQUEST_HPP
