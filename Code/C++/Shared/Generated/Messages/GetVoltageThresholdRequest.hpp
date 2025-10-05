#ifndef GETVOLTAGETHRESHOLDREQUEST_HPP
#define GETVOLTAGETHRESHOLDREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GetVoltageThresholdRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        GetVoltageThresholdRequest(
           uint64_t ticket = 0) noexcept;
        ~GetVoltageThresholdRequest();
        static std::shared_ptr<GetVoltageThresholdRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GETVOLTAGETHRESHOLDREQUEST_HPP
