#ifndef VIRTUALSOCKETMESSAGE_HPP
#define VIRTUALSOCKETMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
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
   public:
        int64_t getId()const noexcept;
        const char* getInternalType()const noexcept;
        const char* getPayload()const noexcept;
        VirtualSocketMessage(
           int64_t id, 
           const char* internalType, 
           const char* payload) noexcept;
        ~VirtualSocketMessage();
        static VirtualSocketMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VIRTUALSOCKETMESSAGE_HPP
