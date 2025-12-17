#pragma once
#include "Generated/VoltageFeedbackModuleConfiguration.hpp"
#include "System/Aborter.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr Configuration FirstStageVoltageFeedbackModuleConfig1{
    .broadcastFrequencyHz = 2,
    .defaultThreshold = 14255.604,
    .vHvOverVadcRatio = 5600
};
inline const uint32_t CONFIG_CRC32_EXPECTED = 1513659569;
inline Configuration FirstStageVoltageFeedbackModuleConfig2 = FirstStageVoltageFeedbackModuleConfig1;//This one is in RAM. FirstStageVoltageFeedbackModuleConfig1 is in ROM.
inline bool validateConfiguration(){
    uint32_t podConfig1 = Crc32::computePod(FirstStageVoltageFeedbackModuleConfig1);
    uint32_t podConfig2 = Crc32::computePod(FirstStageVoltageFeedbackModuleConfig2);
    if (podConfig1 != podConfig2){
        Aborter::safeAbort("Configuration", "The CRC32 computed for FirstStageVoltageFeedbackModuleConfig2 did not match the CRC32 for FirstStageVoltageFeedbackModuleConfig1");
        return false;
    }
    if (podConfig1 != CONFIG_CRC32_EXPECTED){
        Aborter::safeAbort("Configuration", "The CRC32 computed for FirstStageVoltageFeedbackModuleConfig1 did not match the expected value");
        return false;
    }
    return true;
}
