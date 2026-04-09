using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    public struct HVPSConfiguration
    {
        // float fields
        public float broadcastFrequencyHz;
        public float primaryCurrentFromRaw;
        public float firstStageVoltageFromRaw;
        public float outputVoltageFromRaw;
        public float firstStageVoltageThresholdVolts;
        public float maxAverageOutputPowerWatts;
        public float maxFlybackEnergyPerCycleJouls;
        public float maxOutputVoltageThresholdVolts;
        public float defaultOutputVoltageVolts;
        public float maxTemperatureMosfetDegreesC;
        public float minOutputVoltageThresholdVolts;
        public float vPsOverVadcRatio;
        public float villardCapacitorCapacitanceFarads;

        public byte nVillardStages;

        /*public ulong offTimeMicroSeconds;
        public ulong onTimeMicroSeconds;
        */
        public uint pingTimeoutMilliseconds;
        public uint sendPingIntervalMilliseconds;
        public uint villardCapacitorsBleedTimeConstantSeconds;

        public UInt32 idleColour;
        public UInt32 liveColour;
        public UInt32 runningSystemChecksColour;
        public UInt32 shuttingDownColour;
        public UInt32 shutDownColour;
        public UInt32 errorColour;
        public UInt32 unknownColour;
        public UInt32 idleFlashDelayMs;
        public UInt32 liveFlashDelayMs;
        public UInt32 runningSystemChecksFlashDelayMs;
        public UInt32 shuttingDownFlashDelayMs;
        public UInt32 shutDownFlashDelayMs;
        public UInt32 errorFlashDelayMs;
        public UInt32 unknownFlashDelayMs;
    }
}