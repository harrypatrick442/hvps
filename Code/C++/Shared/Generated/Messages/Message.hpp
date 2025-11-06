#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class Message
{
   public:
       static const char* TYPE;
   private:
        const char* _content;
   public:
        const char* getContent() noexcept;
        Message(
           const char* content) noexcept;
        ~Message();
        static std::shared_ptr<Message> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //MESSAGE_HPP
