#ifndef NEWVIRTUALSOCKET2_HPP
#define NEWVIRTUALSOCKET2_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
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
        int64_t getEndpointId() noexcept;
        const char* getSecret() noexcept;
        int64_t getTheirNodeId() noexcept;
        NewVirtualSocket2(
           int64_t endpointId, 
           const char* secret, 
           int64_t theirNodeId) noexcept;
        ~NewVirtualSocket2();
        static std::shared_ptr<NewVirtualSocket2> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NEWVIRTUALSOCKET2_HPP
