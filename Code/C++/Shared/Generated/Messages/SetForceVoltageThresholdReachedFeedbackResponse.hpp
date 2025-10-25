#ifndef SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKRESPONSE_HPP
#define SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetForceVoltageThresholdReachedFeedbackResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        SetForceVoltageThresholdReachedFeedbackResponse(
           uint64_t ticket) noexcept;
        ~SetForceVoltageThresholdReachedFeedbackResponse();
        static std::shared_ptr<SetForceVoltageThresholdReachedFeedbackResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETFORCEVOLTAGETHRESHOLDREACHEDFEEDBACKRESPONSE_HPP
