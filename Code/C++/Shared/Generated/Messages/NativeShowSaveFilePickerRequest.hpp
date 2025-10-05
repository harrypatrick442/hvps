#ifndef NATIVESHOWSAVEFILEPICKERREQUEST_HPP
#define NATIVESHOWSAVEFILEPICKERREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeShowSaveFilePickerRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        NativeShowSaveFilePickerRequest(
           uint64_t ticket = 0) noexcept;
        ~NativeShowSaveFilePickerRequest();
        static std::shared_ptr<NativeShowSaveFilePickerRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESHOWSAVEFILEPICKERREQUEST_HPP
