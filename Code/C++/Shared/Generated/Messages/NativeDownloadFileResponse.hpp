#ifndef NATIVEDOWNLOADFILERESPONSE_HPP
#define NATIVEDOWNLOADFILERESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeDownloadFileResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeDownloadFileResponse(
           uint64_t ticket) noexcept;
        ~NativeDownloadFileResponse();
        static NativeDownloadFileResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEDOWNLOADFILERESPONSE_HPP
