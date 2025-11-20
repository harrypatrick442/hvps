#ifndef NATIVESTORAGESETSTRINGREQUEST_HPP
#define NATIVESTORAGESETSTRINGREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageSetStringRequest
{
   public:
       static const char* TYPE;
   private:
        const char* _key;
        const char* _value;
        uint64_t _ticket;
   public:
        const char* getKey()const noexcept;
        const char* getValue()const noexcept;
        uint64_t getTicket()const noexcept;
        NativeStorageSetStringRequest(
           const char* key, 
           const char* value, 
           uint64_t ticket = 0) noexcept;
        ~NativeStorageSetStringRequest();
        static NativeStorageSetStringRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGESETSTRINGREQUEST_HPP
