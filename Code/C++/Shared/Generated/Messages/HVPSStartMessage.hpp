#ifndef HVPSSTARTMESSAGE_HPP
#define HVPSSTARTMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
class HVPSStartMessage
{
   public:
       static const char* TYPE;
   private:
   public:
        HVPSStartMessage(
) noexcept;
        ~HVPSStartMessage();
        static std::shared_ptr<HVPSStartMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HVPSSTARTMESSAGE_HPP
