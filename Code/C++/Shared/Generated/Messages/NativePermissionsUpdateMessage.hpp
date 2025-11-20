#ifndef NATIVEPERMISSIONSUPDATEMESSAGE_HPP
#define NATIVEPERMISSIONSUPDATEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativePermissionsUpdateMessage
{
   public:
       static const char* TYPE;
   private:
        bool _hasAllRequired;
   public:
        bool getHasAllRequired()const noexcept;
        NativePermissionsUpdateMessage(
           bool hasAllRequired) noexcept;
        ~NativePermissionsUpdateMessage();
        static NativePermissionsUpdateMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPERMISSIONSUPDATEMESSAGE_HPP
