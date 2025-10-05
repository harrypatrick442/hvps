#ifndef PINGMESSAGE_HPP
#define PINGMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class PingMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        PingMessage(
) noexcept;
        ~PingMessage();
        static std::shared_ptr<PingMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //PINGMESSAGE_HPP
