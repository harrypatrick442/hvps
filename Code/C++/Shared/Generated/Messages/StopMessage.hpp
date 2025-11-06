#ifndef STOPMESSAGE_HPP
#define STOPMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class StopMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        StopMessage(
) noexcept;
        ~StopMessage();
        static std::shared_ptr<StopMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STOPMESSAGE_HPP
