#ifndef NATIVESHOWSAVEFILEPICKERRESPONSE_HPP
#define NATIVESHOWSAVEFILEPICKERRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeShowSaveFilePickerResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket()const noexcept;
        NativeShowSaveFilePickerResponse(
           uint64_t ticket) noexcept;
        ~NativeShowSaveFilePickerResponse();
        static NativeShowSaveFilePickerResponse* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESHOWSAVEFILEPICKERRESPONSE_HPP
