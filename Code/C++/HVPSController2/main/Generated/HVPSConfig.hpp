#pragma once
#include "HVPSConfiguration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr HVPSConfiguration Config1{
    .broadcastFrequencyHz = 2,
    .currentSenseVoltageToCurrentAmps = 17.543858,
    .firstStageVoltageThresholdVolts = 14255.604,
    .maxAverageOutputPowerWatts = 12.5,
    .maxFlybackEnergyPerCycleJouls = 0.0098,
    .maxOutputVoltageThresholdVolts = 80000,
    .maxTemperatureLowerSnubberDiodeDegreesC = 50,
    .maxTemperatureMosfetDegreesC = 50,
    .minOutputVoltageThresholdVolts = 60000,
    .vPsOverVadcRatio = 33,
    .villardCapacitorCapacitanceFarads = 1E-09,
    .nVillardStages = 6,
    .offTimeMicroSeconds = 31,
    .onTimeMicroSeconds = 31,
    .pingTimeoutMilliseconds = 3000,
    .villardCapacitorsBleedTimeConstantSeconds = 19
};
inline HVPSConfiguration Config2 = Config1;//This one is in RAM. Config1 is in ROM.
inline bool validateConfig(){
    uint32_t podConfig1 = Crc32::computePod(Config1);
    uint32_t podConfig2 = Crc32::computePod(Config2);
    if (podConfig1 != podConfig2){
        SAFE_ABORT("The CRC32 computed for Config2 did not match the CRC32 for Config1");
        return false;
    }
    if (podConfig1 != 1684788341){
        SAFE_ABORT("The CRC32 computed for Config1 did not match the expected value");
        return false;
    }
    return true;
}
