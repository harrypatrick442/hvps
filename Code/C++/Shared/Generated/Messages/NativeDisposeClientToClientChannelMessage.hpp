#ifndef NATIVEDISPOSECLIENTTOCLIENTCHANNELMESSAGE_HPP
#define NATIVEDISPOSECLIENTTOCLIENTCHANNELMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class NativeDisposeClientToClientChannelMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeDisposeClientToClientChannelMessage(
) noexcept;
        ~NativeDisposeClientToClientChannelMessage();
        static NativeDisposeClientToClientChannelMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEDISPOSECLIENTTOCLIENTCHANNELMESSAGE_HPP
