#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr VoltageFeedbackModuleConfiguration FirstStageVoltageFeedbackModuleConfig1{
    .broadcastFrequencyHz = 2,
    .defaultThreshold = 14255.604,
    .vHvOverVadcRatio = 5600
};
inline VoltageFeedbackModuleConfiguration FirstStageVoltageFeedbackModuleConfig2 = FirstStageVoltageFeedbackModuleConfig1;//This one is in RAM. FirstStageVoltageFeedbackModuleConfig1 is in ROM.
inline bool validateFirstStageVoltageFeedbackModuleConfig(){
    uint32_t podConfig1 = Crc32::computePod(FirstStageVoltageFeedbackModuleConfig1);
    uint32_t podConfig2 = Crc32::computePod(FirstStageVoltageFeedbackModuleConfig2);
    if (podConfig1 != podConfig2){
        SAFE_ABORT("The CRC32 computed for FirstStageVoltageFeedbackModuleConfig2 did not match the CRC32 for FirstStageVoltageFeedbackModuleConfig1");
        return false;
    }
    if (podConfig1 != 1513659569){
        SAFE_ABORT("The CRC32 computed for FirstStageVoltageFeedbackModuleConfig1 did not match the expected value");
        return false;
    }
    return true;
}
