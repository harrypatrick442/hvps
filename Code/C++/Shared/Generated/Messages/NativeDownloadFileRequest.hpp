#ifndef NATIVEDOWNLOADFILEREQUEST_HPP
#define NATIVEDOWNLOADFILEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeDownloadFileRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeDownloadFileRequest(
           uint64_t ticket = 0) noexcept;
        ~NativeDownloadFileRequest();
        static NativeDownloadFileRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEDOWNLOADFILEREQUEST_HPP
