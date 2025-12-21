#pragma once
#include "HVPSConfiguration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr HVPSConfiguration HVPSConfig1{
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
inline HVPSConfiguration HVPSConfig2 = HVPSConfig1;//This one is in RAM. HVPSConfig1 is in ROM.
inline bool validateHVPSConfig(){
    uint32_t podConfig1 = Crc32::computePod(HVPSConfig1);
    uint32_t podConfig2 = Crc32::computePod(HVPSConfig2);
    if (podConfig1 != podConfig2){
        SAFE_ABORT("The CRC32 computed for HVPSConfig2 did not match the CRC32 for HVPSConfig1");
        return false;
    }
    if (podConfig1 != 4277806531){
        SAFE_ABORT("The CRC32 computed for HVPSConfig1 did not match the expected value");
        return false;
    }
    return true;
}
