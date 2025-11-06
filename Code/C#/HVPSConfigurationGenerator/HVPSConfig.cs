using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit, Size = 72)] // Explicit size is helpful
    public struct HVPSConfig
    {
        [FieldOffset(0)] public ulong onTimeMicroSeconds;
        [FieldOffset(8)] public ulong offTimeMicroSeconds;
        [FieldOffset(16)] public double maxOutputVoltageThreshold;
        [FieldOffset(24)] public double minOutputVoltageThreshold;
        [FieldOffset(32)] public double firstStageVoltageThreshold;
        [FieldOffset(40)] public double maxAverageOutputPower;
        [FieldOffset(48)] public byte nStages;
        [FieldOffset(49)] private byte pad1;
        [FieldOffset(50)] private byte pad2;
        [FieldOffset(51)] private byte pad3;
        [FieldOffset(52)] private byte pad4;
        [FieldOffset(53)] private byte pad5;
        [FieldOffset(54)] private byte pad6;
        [FieldOffset(55)] private byte pad7;
        [FieldOffset(56)] public double broadcastFrequencyHz;
        [FieldOffset(64)] public double vPsOverVadcRatio;
        [FieldOffset(72)] public ulong pingTimeoutMilliseconds;

    }

}
