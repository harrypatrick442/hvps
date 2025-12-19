#ifndef SETVOLTAGETHRESHOLDREQUEST_HPP
#define SETVOLTAGETHRESHOLDREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetVoltageThresholdRequest
{
   public:
       static const char* TYPE;
   private:
        float _voltage;
        uint64_t _ticket;
   public:
        float getVoltage()const noexcept;
        uint64_t getTicket()const noexcept;
        SetVoltageThresholdRequest(
           float voltage, 
           uint64_t ticket = 0) noexcept;
        ~SetVoltageThresholdRequest();
        static SetVoltageThresholdRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETVOLTAGETHRESHOLDREQUEST_HPP
