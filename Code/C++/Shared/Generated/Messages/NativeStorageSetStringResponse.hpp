#ifndef NATIVESTORAGESETSTRINGRESPONSE_HPP
#define NATIVESTORAGESETSTRINGRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageSetStringResponse
{
   public:
       static const char* TYPE;
   private:
        uint64_t _ticket;
   public:
        uint64_t getTicket() noexcept;
        NativeStorageSetStringResponse(
           uint64_t ticket) noexcept;
        ~NativeStorageSetStringResponse();
        static std::shared_ptr<NativeStorageSetStringResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGESETSTRINGRESPONSE_HPP
