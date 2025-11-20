#ifndef NATIVEREADYMESSAGE_HPP
#define NATIVEREADYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class NativeReadyMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeReadyMessage(
) noexcept;
        ~NativeReadyMessage();
        static NativeReadyMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEREADYMESSAGE_HPP
