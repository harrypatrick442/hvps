#ifndef NATIVESTORAGEGETSTRINGRESPONSE_HPP
#define NATIVESTORAGEGETSTRINGRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageGetStringResponse
{
   public:
       static const char* TYPE;
   private:
        const char* _value;
        uint64_t _ticket;
   public:
        const char* getValue()const noexcept;
        uint64_t getTicket()const noexcept;
        NativeStorageGetStringResponse(
           const char* value, 
           uint64_t ticket) noexcept;
        ~NativeStorageGetStringResponse();
        static NativeStorageGetStringResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGEGETSTRINGRESPONSE_HPP
