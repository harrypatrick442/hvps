#ifndef NATIVEOPENDIRECTORYREQUEST_HPP
#define NATIVEOPENDIRECTORYREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeOpenDirectoryRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeOpenDirectoryRequest(
           uint64_t ticket = 0) noexcept;
        ~NativeOpenDirectoryRequest();
        static NativeOpenDirectoryRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEOPENDIRECTORYREQUEST_HPP
