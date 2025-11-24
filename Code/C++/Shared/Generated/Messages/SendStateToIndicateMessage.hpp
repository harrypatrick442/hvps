#ifndef SENDSTATETOINDICATEMESSAGE_HPP
#define SENDSTATETOINDICATEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
class SendStateToIndicateMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        SendStateToIndicateMessage(
) noexcept;
        ~SendStateToIndicateMessage();
        static SendStateToIndicateMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SENDSTATETOINDICATEMESSAGE_HPP
