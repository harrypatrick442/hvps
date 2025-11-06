#ifndef STOPMESSAGEREQUEST_HPP
#define STOPMESSAGEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class StopMessageRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        StopMessageRequest(
           uint64_t ticket = 0) noexcept;
        ~StopMessageRequest();
        static std::shared_ptr<StopMessageRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STOPMESSAGEREQUEST_HPP
