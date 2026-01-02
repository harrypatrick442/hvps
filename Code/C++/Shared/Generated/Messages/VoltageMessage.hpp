#ifndef VOLTAGEMESSAGE_HPP
#define VOLTAGEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class VoltageMessage
{
   public:
       static const char* TYPE;
   private:
        uint16_t _rawVoltage;
        float _voltage;
   public:
        uint16_t getRawVoltage()const noexcept;
        float getVoltage()const noexcept;
        VoltageMessage(
           uint16_t rawVoltage, 
           float voltage) noexcept;
        ~VoltageMessage();
        static VoltageMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VOLTAGEMESSAGE_HPP
