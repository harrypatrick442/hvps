#ifndef NATIVESTORAGESETSTRINGRESPONSE_HPP
#define NATIVESTORAGESETSTRINGRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageSetStringResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeStorageSetStringResponse(
           uint64_t ticket) noexcept;
        ~NativeStorageSetStringResponse();
        static NativeStorageSetStringResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGESETSTRINGRESPONSE_HPP
