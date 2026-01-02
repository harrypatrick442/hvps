#ifndef SETVOLTAGETHRESHOLDRESPONSE_HPP
#define SETVOLTAGETHRESHOLDRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetVoltageThresholdResponse
{
   public:
       static const char* TYPE;
   private:
        float _voltage;
        uint64_t _ticket;
   public:
        float getVoltage()const noexcept;
        uint64_t getTicket()const noexcept;
        SetVoltageThresholdResponse(
           float voltage, 
           uint64_t ticket) noexcept;
        ~SetVoltageThresholdResponse();
        static SetVoltageThresholdResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETVOLTAGETHRESHOLDRESPONSE_HPP
