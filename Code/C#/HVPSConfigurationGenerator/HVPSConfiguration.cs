using System.Runtime.InteropServices;

namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit, Size = 72)]
    public struct HVPSConfiguration
    {
        // ─────────────────────────────
        // float fields (4 bytes each)
        // ─────────────────────────────

        // 0–3
        [FieldOffset(0)]
        public float broadcastFrequencyHz;

        // 4–7
        [FieldOffset(4)]
        public float currentSenseVoltageToCurrentAmps;

        // 8–11
        [FieldOffset(8)]
        public float firstStageVoltageThresholdVolts;

        // 12–15
        [FieldOffset(12)]
        public float maxAverageOutputPowerWatts;

        // 16–19
        [FieldOffset(16)]
        public float maxFlybackEnergyPerCycleJouls;

        // 20–23
        [FieldOffset(20)]
        public float maxOutputVoltageThresholdVolts;

        // 24–27
        [FieldOffset(24)]
        public float maxTemperatureLowerSnubberDiodeDegreesC;

        // 28–31
        [FieldOffset(28)]
        public float maxTemperatureMosfetDegreesC;

        // 32–35
        [FieldOffset(32)]
        public float minOutputVoltageThresholdVolts;

        // 36–39
        [FieldOffset(36)]
        public float vPsOverVadcRatio;

        // 40–43
        [FieldOffset(40)]
        public float villardCapacitorCapacitanceFarads;

        // ─────────────────────────────
        // byte + padding (align ulongs)
        // ─────────────────────────────

        // 44
        [FieldOffset(44)]
        public byte nVillardStages;

        // 45–47 padding → next 8-byte boundary
        [FieldOffset(45)] private byte pad1;
        [FieldOffset(46)] private byte pad2;
        [FieldOffset(47)] private byte pad3;

        // ─────────────────────────────
        // ulong fields (8 bytes each)
        // ─────────────────────────────

        // 48–55
        [FieldOffset(48)]
        public ulong offTimeMicroSeconds;

        // 56–63
        [FieldOffset(56)]
        public ulong onTimeMicroSeconds;

        // ─────────────────────────────
        // uint field
        // ─────────────────────────────

        // 64–67
        [FieldOffset(64)]
        public uint pingTimeoutMilliseconds;

        // 68–71
        [FieldOffset(68)]
        public uint villardCapacitorsBleedTimeConstantSeconds;
    }
}
