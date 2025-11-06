#ifndef SHUTDOWNMESSAGE_HPP
#define SHUTDOWNMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class ShutDownMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        ShutDownMessage(
) noexcept;
        ~ShutDownMessage();
        static std::shared_ptr<ShutDownMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //SHUTDOWNMESSAGE_HPP
