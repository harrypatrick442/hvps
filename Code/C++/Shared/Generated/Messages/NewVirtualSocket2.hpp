#ifndef NEWVIRTUALSOCKET2_HPP
#define NEWVIRTUALSOCKET2_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NewVirtualSocket2
{
   public:
       static const char* TYPE;
   private:
        int64_t _endpointId;
        const char* _secret;
        int64_t _theirNodeId;
   public:
        int64_t getEndpointId()const noexcept;
        const char* getSecret()const noexcept;
        int64_t getTheirNodeId()const noexcept;
        NewVirtualSocket2(
           int64_t endpointId, 
           const char* secret, 
           int64_t theirNodeId) noexcept;
        ~NewVirtualSocket2();
        static NewVirtualSocket2* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NEWVIRTUALSOCKET2_HPP
