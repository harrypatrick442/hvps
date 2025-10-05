#ifndef SETVOLTAGETHRESHOLDRESPONSE_HPP
#define SETVOLTAGETHRESHOLDRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class SetVoltageThresholdResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        SetVoltageThresholdResponse(
           uint64_t ticket) noexcept;
        ~SetVoltageThresholdResponse();
        static std::shared_ptr<SetVoltageThresholdResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SETVOLTAGETHRESHOLDRESPONSE_HPP
