#ifndef VIRTUALSOCKET2MESSAGE_HPP
#define VIRTUALSOCKET2MESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
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
        std::optional<int64_t> getEndpointId() noexcept;
        const char* getPayload() noexcept;
        const char* getSecret() noexcept;
        std::optional<int32_t> getTheirNodeId() noexcept;
        VirtualSocket2Message(
           std::optional<int64_t> endpointId, 
           const char* payload, 
           const char* secret, 
           std::optional<int32_t> theirNodeId) noexcept;
        ~VirtualSocket2Message();
        static std::shared_ptr<VirtualSocket2Message> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VIRTUALSOCKET2MESSAGE_HPP
