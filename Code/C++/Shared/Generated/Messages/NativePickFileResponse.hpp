#ifndef NATIVEPICKFILERESPONSE_HPP
#define NATIVEPICKFILERESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativePickFileResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        NativePickFileResponse(
           uint64_t ticket) noexcept;
        ~NativePickFileResponse();
        static std::shared_ptr<NativePickFileResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPICKFILERESPONSE_HPP
