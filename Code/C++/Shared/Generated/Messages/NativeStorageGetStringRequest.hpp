#ifndef NATIVESTORAGEGETSTRINGREQUEST_HPP
#define NATIVESTORAGEGETSTRINGREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageGetStringRequest
{
   public:
       static const char* TYPE;
   private:
        const char* _key;
        uint64_t _ticket;
        bool _freeMemoryInDeconstructor;
   public:
        const char* getKey() noexcept;
        uint64_t getTicket() noexcept;
        NativeStorageGetStringRequest(
           const char* key, 
           uint64_t ticket = 0) noexcept;
        ~NativeStorageGetStringRequest();
        static std::shared_ptr<NativeStorageGetStringRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGEGETSTRINGREQUEST_HPP
