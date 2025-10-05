#ifndef GETVOLTAGEREQUEST_HPP
#define GETVOLTAGEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GetVoltageRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        GetVoltageRequest(
           uint64_t ticket = 0) noexcept;
        ~GetVoltageRequest();
        static std::shared_ptr<GetVoltageRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GETVOLTAGEREQUEST_HPP
