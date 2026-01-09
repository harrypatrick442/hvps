#ifndef LIVEDATAMESSAGE_HPP
#define LIVEDATAMESSAGE_HPP

#include "../../cJSON/cJSON.h"
#include "../../JSON/JHelper.hpp"
#include "../../Core/CleanupBucket.hpp"
#include <memory>
#include "../../JSON/JHelper.hpp"
class LiveDataMessage
{
   public:
       static const char* TYPE;
   private:
        uint8_t _firstStageVoltageValueBoundType;
        float _firstStageVoltageVolts;
        float _frequencyHz;
        uint8_t _frequencyHzValueBoundType;
        float _outputCurrentAmps;
        uint8_t _outputVoltageValueBoundType;
        float _outputVoltageVolts;
        float _peakPrimaryCurrentAmps;
        float _primaryPowerWatts;
        float _totalOutputEnergyJouls;
        float _totalPrimaryEnergyJouls;
   public:
        uint8_t getFirstStageVoltageValueBoundType()const noexcept;
        float getFirstStageVoltageVolts()const noexcept;
        float getFrequencyHz()const noexcept;
        uint8_t getFrequencyHzValueBoundType()const noexcept;
        float getOutputCurrentAmps()const noexcept;
        uint8_t getOutputVoltageValueBoundType()const noexcept;
        float getOutputVoltageVolts()const noexcept;
        float getPeakPrimaryCurrentAmps()const noexcept;
        float getPrimaryPowerWatts()const noexcept;
        float getTotalOutputEnergyJouls()const noexcept;
        float getTotalPrimaryEnergyJouls()const noexcept;
        LiveDataMessage(
           uint8_t firstStageVoltageValueBoundType, 
           float firstStageVoltageVolts, 
           float frequencyHz, 
           uint8_t frequencyHzValueBoundType, 
           float outputCurrentAmps, 
           uint8_t outputVoltageValueBoundType, 
           float outputVoltageVolts, 
           float peakPrimaryCurrentAmps, 
           float primaryPowerWatts, 
           float totalOutputEnergyJouls, 
           float totalPrimaryEnergyJouls) noexcept;
        ~LiveDataMessage();
        static LiveDataMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //LIVEDATAMESSAGE_HPP
