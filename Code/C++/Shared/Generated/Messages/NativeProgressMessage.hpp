#ifndef NATIVEPROGRESSMESSAGE_HPP
#define NATIVEPROGRESSMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class NativeProgressMessage
{
   private:
   public:
        NativeProgressMessage(
) noexcept;
        ~NativeProgressMessage();
        static NativeProgressMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPROGRESSMESSAGE_HPP
