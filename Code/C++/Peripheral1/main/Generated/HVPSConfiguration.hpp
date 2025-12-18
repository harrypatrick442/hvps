#pragma once
#include <cstdint>
struct HVPSConfiguration {
    const float broadcastFrequencyHz;
    const float firstStageVoltageThresholdVolts;
    const float maxAverageOutputPowerWatts;
    const float maxFlybackEnergyPerCycleJouls;
    const float maxOutputVoltageThresholdVolts;
    const float minOutputVoltageThresholdVolts;
    const float vPsOverVadcRatio;
    const float villardCapacitorCapacitanceFarads;
    const uint8_t nVillardStages;
    const uint64_t offTimeMicroSeconds;
    const uint64_t onTimeMicroSeconds;
    const uint64_t pingTimeoutMilliseconds;
};
