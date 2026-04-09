#pragma once
#include "HVPSConfiguration.hpp"
#include "System/SafeAbort.hpp"
#include "Core/Checksums/Crc32.hpp"
inline constexpr HVPSConfiguration Config1{
    .broadcastFrequencyHz = 2,
    .primaryCurrentFromRaw = 0.15686275,
    .firstStageVoltageFromRaw = 65.882355,
    .outputVoltageFromRaw = 241.56863,
    .firstStageVoltageThresholdVolts = 14255.604,
    .maxAverageOutputPowerWatts = 12.5,
    .maxFlybackEnergyPerCycleJouls = 0.0098,
    .maxOutputVoltageThresholdVolts = 80000,
    .defaultOutputVoltageVolts = 80000,
    .maxTemperatureMosfetDegreesC = 50,
    .minOutputVoltageThresholdVolts = 60000,
    .vPsOverVadcRatio = 16.5,
    .villardCapacitorCapacitanceFarads = 1E-09,
    .nVillardStages = 6,
    .pingTimeoutMilliseconds = 10000,
    .sendPingIntervalMilliseconds = 2000,
    .villardCapacitorsBleedTimeConstantSeconds = 19,
    .idleColour = 14724644,
    .liveColour = 255,
    .runningSystemChecksColour = 5631,
    .shuttingDownColour = 16523,
    .shutDownColour = 65280,
    .errorColour = 6227959,
    .unknownColour = 16777215,
    .idleFlashDelayMs = 250,
    .liveFlashDelayMs = 250,
    .runningSystemChecksFlashDelayMs = 500,
    .shuttingDownFlashDelayMs = 500,
    .shutDownFlashDelayMs = 0,
    .errorFlashDelayMs = 500,
    .unknownFlashDelayMs = 500
};
inline HVPSConfiguration Config2 = Config1;//This one is in RAM. Config1 is in ROM.
inline bool validateConfig(){
    uint32_t podConfig1 = Crc32::computePod(Config1);
    uint32_t podConfig2 = Crc32::computePod(Config2);
    if (podConfig1 != podConfig2){
        SAFE_ABORT("The CRC32 computed for Config2 did not match the CRC32 for Config1");
        return false;
    }
    if (podConfig1 != 506985598){
        SAFE_ABORT("The CRC32 computed for Config1 did not match the expected value");
        return false;
    }
    return true;
}
