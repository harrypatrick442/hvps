#ifndef HVPSSTOPMESSAGE_HPP
#define HVPSSTOPMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class HVPSStopMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        HVPSStopMessage(
) noexcept;
        ~HVPSStopMessage();
        static std::shared_ptr<HVPSStopMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HVPSSTOPMESSAGE_HPP
