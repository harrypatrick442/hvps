#ifndef NATIVEPLATFORMMESSAGE_HPP
#define NATIVEPLATFORMMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativePlatformMessage
{
   public:
       static const char* TYPE;
   private:
        int32_t _platform;
   public:
        int32_t getPlatform()const noexcept;
        NativePlatformMessage(
           int32_t platform) noexcept;
        ~NativePlatformMessage();
        static NativePlatformMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPLATFORMMESSAGE_HPP
