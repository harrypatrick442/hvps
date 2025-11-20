#ifndef NATIVESTORAGEGETSTRINGREQUEST_HPP
#define NATIVESTORAGEGETSTRINGREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageGetStringRequest
{
   public:
       static const char* TYPE;
   private:
        const char* _key;
        uint64_t _ticket;
   public:
        const char* getKey()const noexcept;
        uint64_t getTicket()const noexcept;
        NativeStorageGetStringRequest(
           const char* key, 
           uint64_t ticket = 0) noexcept;
        ~NativeStorageGetStringRequest();
        static NativeStorageGetStringRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGEGETSTRINGREQUEST_HPP
