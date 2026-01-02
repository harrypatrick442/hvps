#ifndef SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKRESPONSE_HPP
#define SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetForceVoltageThresholdReachedFeedbackResponse
{
   public:
       static const char* TYPE;
   private:
        std::optional<bool> _force;
        uint64_t _ticket;
   public:
        std::optional<bool> getForce()const noexcept;
        uint64_t getTicket()const noexcept;
        SetForceVoltageThresholdReachedFeedbackResponse(
           std::optional<bool> force, 
           uint64_t ticket) noexcept;
        ~SetForceVoltageThresholdReachedFeedbackResponse();
        static SetForceVoltageThresholdReachedFeedbackResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKRESPONSE_HPP
