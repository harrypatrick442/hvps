#ifndef SETVOLTAGETHRESHOLDREQUEST_HPP
#define SETVOLTAGETHRESHOLDREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetVoltageThresholdRequest
{
   public:
       static const char* TYPE;
   private:
        double _voltage;
        uint64_t _ticket;
   public:
        double getVoltage() noexcept;
        uint64_t getTicket() noexcept;
        SetVoltageThresholdRequest(
           double voltage, 
           uint64_t ticket = 0) noexcept;
        ~SetVoltageThresholdRequest();
        static std::shared_ptr<SetVoltageThresholdRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETVOLTAGETHRESHOLDREQUEST_HPP
