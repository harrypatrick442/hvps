#ifndef GREETINGREQUEST_HPP
#define GREETINGREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class GreetingRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        GreetingRequest(
           uint64_t ticket = 0) noexcept;
        ~GreetingRequest();
        static std::shared_ptr<GreetingRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //GREETINGREQUEST_HPP
