#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class Message
{
   public:
       static const char* TYPE;
   private:
        const char* _content;
   public:
        const char* getContent()const noexcept;
        Message(
           const char* content) noexcept;
        ~Message();
        static Message* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //MESSAGE_HPP
