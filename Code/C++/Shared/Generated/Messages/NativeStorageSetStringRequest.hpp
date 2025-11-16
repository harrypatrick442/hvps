#ifndef NATIVESTORAGESETSTRINGREQUEST_HPP
#define NATIVESTORAGESETSTRINGREQUEST_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativeStorageSetStringRequest
{
   public:
       static const char* TYPE;
   private:
        const char* _key;
        const char* _value;
        uint64_t _ticket;
        bool _freeMemoryInDeconstructor;
   public:
        const char* getKey() noexcept;
        const char* getValue() noexcept;
        uint64_t getTicket() noexcept;
        NativeStorageSetStringRequest(
           const char* key, 
           const char* value, 
           uint64_t ticket = 0) noexcept;
        ~NativeStorageSetStringRequest();
        static std::shared_ptr<NativeStorageSetStringRequest> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVESTORAGESETSTRINGREQUEST_HPP
