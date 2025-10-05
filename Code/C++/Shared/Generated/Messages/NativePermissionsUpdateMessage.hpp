#ifndef NATIVEPERMISSIONSUPDATEMESSAGE_HPP
#define NATIVEPERMISSIONSUPDATEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class NativePermissionsUpdateMessage
{
   public:
       static const char* TYPE;
   private:
        bool _hasAllRequired;
   public:
        bool getHasAllRequired() noexcept;
        NativePermissionsUpdateMessage(
           bool hasAllRequired) noexcept;
        ~NativePermissionsUpdateMessage();
        static std::shared_ptr<NativePermissionsUpdateMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEPERMISSIONSUPDATEMESSAGE_HPP
