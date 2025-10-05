#ifndef HVPSRUNSYSTEMCHECKSONLYMESSAGE_HPP
#define HVPSRUNSYSTEMCHECKSONLYMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class HVPSRunSystemChecksOnlyMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        HVPSRunSystemChecksOnlyMessage(
) noexcept;
        ~HVPSRunSystemChecksOnlyMessage();
        static std::shared_ptr<HVPSRunSystemChecksOnlyMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HVPSRUNSYSTEMCHECKSONLYMESSAGE_HPP
