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
        float _firstStageVoltage;
        std::optional<uint32_t> _frequency;
        float _outputCurrent;
        float _outputVoltage;
        float _peakPrimaryCurrent;
        float _totalOutputEnergy;
   public:
        float getFirstStageVoltage()const noexcept;
        std::optional<uint32_t> getFrequency()const noexcept;
        float getOutputCurrent()const noexcept;
        float getOutputVoltage()const noexcept;
        float getPeakPrimaryCurrent()const noexcept;
        float getTotalOutputEnergy()const noexcept;
        LiveDataMessage(
           float firstStageVoltage, 
           std::optional<uint32_t> frequency, 
           float outputCurrent, 
           float outputVoltage, 
           float peakPrimaryCurrent, 
           float totalOutputEnergy) noexcept;
        ~LiveDataMessage();
        static LiveDataMessage* fromJSON(cJSON* j, CleanupBucket& cleanupBucket) noexcept;
        cJSON* toJSON() noexcept;
};
#endif //LIVEDATAMESSAGE_HPP
