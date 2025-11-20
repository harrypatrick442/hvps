#ifndef NATIVECLOSEFILEMESSAGE_HPP
#define NATIVECLOSEFILEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class NativeCloseFileMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeCloseFileMessage(
) noexcept;
        ~NativeCloseFileMessage();
        static NativeCloseFileMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVECLOSEFILEMESSAGE_HPP
