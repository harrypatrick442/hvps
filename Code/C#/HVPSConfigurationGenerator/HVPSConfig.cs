using System.Runtime.InteropServices;

namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit, Size = 64)]
    public struct HVPSConfiguration
    {
        // ─────────────────────────────
        // float fields (alphabetical)
        // ─────────────────────────────

        // 0–3
        [FieldOffset(0)]
        public float broadcastFrequencyHz;

        // 4–7
        [FieldOffset(4)]
        public float firstStageVoltageThresholdVolts;

        // 8–11
        [FieldOffset(8)]
        public float maxAverageOutputPowerWatts;

        // 12–15
        [FieldOffset(12)]
        public float maxFlybackEnergyPerCycleJouls;

        // 16–19
        [FieldOffset(16)]
        public float maxOutputVoltageThresholdVolts;

        // 20–23
        [FieldOffset(20)]
        public float minOutputVoltageThresholdVolts;

        // 24–27
        [FieldOffset(24)]
        public float vPsOverVadcRatio;

        // 28–31
        [FieldOffset(28)]
        public float villardCapacitorCapacitanceFarads;

        // ─────────────────────────────
        // integer / scalar fields
        // ─────────────────────────────

        // 32
        [FieldOffset(32)]
        public byte nVillardStages;

        // 33–39 padding for 8-byte alignment
        [FieldOffset(33)] private byte pad1;
        [FieldOffset(34)] private byte pad2;
        [FieldOffset(35)] private byte pad3;
        [FieldOffset(36)] private byte pad4;
        [FieldOffset(37)] private byte pad5;
        [FieldOffset(38)] private byte pad6;
        [FieldOffset(39)] private byte pad7;

        // ─────────────────────────────
        // ulong fields (alphabetical)
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
    }
}
