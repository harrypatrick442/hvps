#ifndef VOLTAGEMESSAGE_HPP
#define VOLTAGEMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class VoltageMessage
{
   public:
       static const char* TYPE;
   private:
        double _voltage;
   public:
        double getVoltage() noexcept;
        VoltageMessage(
           double voltage) noexcept;
        ~VoltageMessage();
        static std::shared_ptr<VoltageMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //VOLTAGEMESSAGE_HPP
