#ifndef SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKREQUEST_HPP
#define SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetForceVoltageThresholdReachedFeedbackRequest
{
   public:
       static const char* TYPE;
   private:
        bool _force;
        uint64_t _ticket;
   public:
        bool getForce()const noexcept;
        uint64_t getTicket()const noexcept;
        SetForceVoltageThresholdReachedFeedbackRequest(
           bool force, 
           uint64_t ticket = 0) noexcept;
        ~SetForceVoltageThresholdReachedFeedbackRequest();
        static SetForceVoltageThresholdReachedFeedbackRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKREQUEST_HPP
