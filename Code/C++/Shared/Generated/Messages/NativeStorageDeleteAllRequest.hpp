#ifndef NATIVESTORAGEDELETEALLREQUEST_HPP
#define NATIVESTORAGEDELETEALLREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageDeleteAllRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeStorageDeleteAllRequest(
           uint64_t ticket = 0) noexcept;
        ~NativeStorageDeleteAllRequest();
        static NativeStorageDeleteAllRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGEDELETEALLREQUEST_HPP
