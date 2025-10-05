#ifndef HVPSLIVEDATAMESSAGE_HPP
#define HVPSLIVEDATAMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class HVPSLiveDataMessage
{
   public:
       static const char* TYPE;
   private:
        double _firstStageVoltage;
        double _outputCurrent;
        double _outputVoltage;
        double _peakPrimaryCurrent;
        double _totalOutputEnergy;
   public:
        double getFirstStageVoltage() noexcept;
        double getOutputCurrent() noexcept;
        double getOutputVoltage() noexcept;
        double getPeakPrimaryCurrent() noexcept;
        double getTotalOutputEnergy() noexcept;
        HVPSLiveDataMessage(
           double firstStageVoltage, 
           double outputCurrent, 
           double outputVoltage, 
           double peakPrimaryCurrent, 
           double totalOutputEnergy) noexcept;
        ~HVPSLiveDataMessage();
        static std::shared_ptr<HVPSLiveDataMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //HVPSLIVEDATAMESSAGE_HPP
