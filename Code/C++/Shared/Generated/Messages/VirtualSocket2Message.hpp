#ifndef VIRTUALSOCKET2MESSAGE_HPP
#define VIRTUALSOCKET2MESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class VirtualSocket2Message
{
   public:
       static const char* TYPE;
   private:
        std::optional<int64_t> _endpointId;
        const char* _payload;
        const char* _secret;
        std::optional<int32_t> _theirNodeId;
   public:
        std::optional<int64_t> getEndpointId()const noexcept;
        const char* getPayload()const noexcept;
        const char* getSecret()const noexcept;
        std::optional<int32_t> getTheirNodeId()const noexcept;
        VirtualSocket2Message(
           std::optional<int64_t> endpointId, 
           const char* payload, 
           const char* secret, 
           std::optional<int32_t> theirNodeId) noexcept;
        ~VirtualSocket2Message();
        static VirtualSocket2Message* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VIRTUALSOCKET2MESSAGE_HPP
