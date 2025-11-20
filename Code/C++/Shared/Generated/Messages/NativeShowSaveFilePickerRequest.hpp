#ifndef NATIVESHOWSAVEFILEPICKERREQUEST_HPP
#define NATIVESHOWSAVEFILEPICKERREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeShowSaveFilePickerRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeShowSaveFilePickerRequest(
           uint64_t ticket = 0) noexcept;
        ~NativeShowSaveFilePickerRequest();
        static NativeShowSaveFilePickerRequest* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESHOWSAVEFILEPICKERREQUEST_HPP
