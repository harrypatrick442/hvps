#ifndef NATIVEPICKFILEREQUEST_HPP
#define NATIVEPICKFILEREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativePickFileRequest
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        NativePickFileRequest(
           uint64_t ticket = 0) noexcept;
        ~NativePickFileRequest();
        static std::shared_ptr<NativePickFileRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPICKFILEREQUEST_HPP
