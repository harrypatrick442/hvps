#ifndef NATIVEGOTNEWTOKENMESSAGE_HPP
#define NATIVEGOTNEWTOKENMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class NativeGotNewTokenMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        NativeGotNewTokenMessage(
) noexcept;
        ~NativeGotNewTokenMessage();
        static NativeGotNewTokenMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //NATIVEGOTNEWTOKENMESSAGE_HPP
