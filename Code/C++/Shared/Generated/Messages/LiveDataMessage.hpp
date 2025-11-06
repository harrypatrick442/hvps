#ifndef LIVEDATAMESSAGE_HPP
#define LIVEDATAMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class LiveDataMessage
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
        LiveDataMessage(
           double firstStageVoltage, 
           double outputCurrent, 
           double outputVoltage, 
           double peakPrimaryCurrent, 
           double totalOutputEnergy) noexcept;
        ~LiveDataMessage();
        static std::shared_ptr<LiveDataMessage> fromJSON(cJSON* j) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //LIVEDATAMESSAGE_HPP
