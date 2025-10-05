#ifndef NATIVESTORAGEGETSTRINGRESPONSE_HPP
#define NATIVESTORAGEGETSTRINGRESPONSE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageGetStringResponse
{
   public:
       static const char* TYPE;
   private:
        const char* _value;
        uint64_t _ticket;
   public:
        const char* getValue() noexcept;
        uint64_t getTicket() noexcept;
        NativeStorageGetStringResponse(
           const char* value, 
           uint64_t ticket) noexcept;
        ~NativeStorageGetStringResponse();
        static std::shared_ptr<NativeStorageGetStringResponse> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGEGETSTRINGRESPONSE_HPP
