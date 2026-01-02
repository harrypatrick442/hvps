#ifndef GETVOLTAGERESPONSE_HPP
#define GETVOLTAGERESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GetVoltageResponse
{
   public:
       static const char* TYPE;
   private:
        uint16_t _rawVoltage;
        float _voltage;
        uint64_t _ticket;
   public:
        uint16_t getRawVoltage()const noexcept;
        float getVoltage()const noexcept;
        uint64_t getTicket()const noexcept;
        GetVoltageResponse(
           uint16_t rawVoltage, 
           float voltage, 
           uint64_t ticket) noexcept;
        ~GetVoltageResponse();
        static GetVoltageResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GETVOLTAGERESPONSE_HPP
