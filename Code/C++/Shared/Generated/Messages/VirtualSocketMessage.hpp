#ifndef VIRTUALSOCKETMESSAGE_HPP
#define VIRTUALSOCKETMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class VirtualSocketMessage
{
   public:
       static const char* TYPE;
   private:
        int64_t _id;
        const char* _internalType;
        const char* _payload;
        bool _freeMemoryInDeconstructor;
   public:
        int64_t getId() noexcept;
        const char* getInternalType() noexcept;
        const char* getPayload() noexcept;
        VirtualSocketMessage(
           int64_t id, 
           const char* internalType, 
           const char* payload) noexcept;
        ~VirtualSocketMessage();
        static std::shared_ptr<VirtualSocketMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VIRTUALSOCKETMESSAGE_HPP
