#ifndef STARTMESSAGE_HPP
#define STARTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class StartMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        StartMessage(
) noexcept;
        ~StartMessage();
        static std::shared_ptr<StartMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //STARTMESSAGE_HPP
