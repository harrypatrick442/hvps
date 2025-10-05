#ifndef GETVOLTAGETHRESHOLDRESPONSE_HPP
#define GETVOLTAGETHRESHOLDRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GetVoltageThresholdResponse
{
   public:
       static const char* TYPE;
   private:
        double _voltage;
        uint64_t _ticket;
   public:
        double getVoltage() noexcept;
        uint64_t getTicket() noexcept;
        GetVoltageThresholdResponse(
           double voltage, 
           uint64_t ticket) noexcept;
        ~GetVoltageThresholdResponse();
        static std::shared_ptr<GetVoltageThresholdResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GETVOLTAGETHRESHOLDRESPONSE_HPP
