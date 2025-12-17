#pragma once
#include "HVPSConfiguration.hpp"
#include "System/Aborter.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr Configuration HVPSConfig1{
    .broadcastFrequencyHz = 2,
    .firstStageVoltageThresholdVolts = 14255.604,
    .maxAverageOutputPowerWatts = 12.5,
    .maxFlybackEnergyPerCycleJouls = 0.0098,
    .maxOutputVoltageThresholdVolts = 80000,
    .minOutputVoltageThresholdVolts = 60000,
    .vPsOverVadcRatio = 33,
    .villardCapacitorCapacitanceFarads = 1E-09,
    .nVillardStages = 6,
    .offTimeMicroSeconds = 31,
    .onTimeMicroSeconds = 31,
    .pingTimeoutMilliseconds = 3000
};
inline const uint32_t CONFIG_CRC32_EXPECTED = 4277806531;
inline Configuration HVPSConfig2 = HVPSConfig1;//This one is in RAM. HVPSConfig1 is in ROM.
inline bool validateConfiguration(){
    uint32_t podConfig1 = Crc32::computePod(HVPSConfig1);
    uint32_t podConfig2 = Crc32::computePod(HVPSConfig2);
    if (podConfig1 != podConfig2){
        Aborter::safeAbort("Configuration", "The CRC32 computed for HVPSConfig2 did not match the CRC32 for HVPSConfig1");
        return false;
    }
    if (podConfig1 != CONFIG_CRC32_EXPECTED){
        Aborter::safeAbort("Configuration", "The CRC32 computed for HVPSConfig1 did not match the expected value");
        return false;
    }
    return true;
}
