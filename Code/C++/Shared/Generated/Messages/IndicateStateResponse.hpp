#ifndef INDICATESTATERESPONSE_HPP
#define INDICATESTATERESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class IndicateStateResponse
{
   public:
       static const char* TYPE;
   private:
        bool _success;
        uint64_t _ticket;
   public:
        bool getSuccess()const noexcept;
        uint64_t getTicket()const noexcept;
        IndicateStateResponse(
           bool success, 
           uint64_t ticket) noexcept;
        ~IndicateStateResponse();
        static IndicateStateResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //INDICATESTATERESPONSE_HPP
