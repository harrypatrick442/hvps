#ifndef SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKREQUEST_HPP
#define SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
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
        bool getForce() noexcept;
        uint64_t getTicket() noexcept;
        SetForceVoltageThresholdReachedFeedbackRequest(
           bool force, 
           uint64_t ticket = 0) noexcept;
        ~SetForceVoltageThresholdReachedFeedbackRequest();
        static std::shared_ptr<SetForceVoltageThresholdReachedFeedbackRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKREQUEST_HPP
