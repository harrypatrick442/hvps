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
        float _voltage;
   public:
        float getVoltage()const noexcept;
        VoltageMessage(
           float voltage) noexcept;
        ~VoltageMessage();
        static VoltageMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VOLTAGEMESSAGE_HPP
