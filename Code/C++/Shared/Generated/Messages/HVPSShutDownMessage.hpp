#ifndef HVPSSHUTDOWNMESSAGE_HPP
#define HVPSSHUTDOWNMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class HVPSShutDownMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        HVPSShutDownMessage(
) noexcept;
        ~HVPSShutDownMessage();
        static std::shared_ptr<HVPSShutDownMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HVPSSHUTDOWNMESSAGE_HPP
