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
        public float minOutputVoltageThresholdVolts;

        // 28–31
        [FieldOffset(28)]
        public float vPsOverVadcRatio;

        // 32–35
        [FieldOffset(32)]
        public float villardCapacitorCapacitanceFarads;

        // ─────────────────────────────
        // byte + padding (align ulongs)
        // ─────────────────────────────

        // 36
        [FieldOffset(36)]
        public byte nVillardStages;

        // 37–39 padding → next 8-byte boundary
        [FieldOffset(37)] private byte pad1;
        [FieldOffset(38)] private byte pad2;
        [FieldOffset(39)] private byte pad3;

        // ─────────────────────────────
        // ulong fields (8 bytes each)
        // ─────────────────────────────

        // 40–47
        [FieldOffset(40)]
        public ulong offTimeMicroSeconds;

        // 48–55
        [FieldOffset(48)]
        public ulong onTimeMicroSeconds;

        // 56–63
        [FieldOffset(56)]
        public ulong pingTimeoutMilliseconds;

        // ─────────────────────────────
        // uint field
        // ─────────────────────────────

        // 64–67
        [FieldOffset(64)]
        public uint villardCapacitorsBleedTimeConstantSeconds;

        // 68–71 final padding
        [FieldOffset(68)] private byte pad4;
        [FieldOffset(69)] private byte pad5;
        [FieldOffset(70)] private byte pad6;
        [FieldOffset(71)] private byte pad7;
    }
}
