using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit)]
    public struct HVPSConfig
    {
        [FieldOffset(0)] public ulong onTimeMicroSeconds;
        [FieldOffset(8)] public ulong offTimeMicroSeconds;
        [FieldOffset(16)] public double maxOutputVoltageThreshold;
        [FieldOffset(24)] public double minOutputVoltageThreshold;
        [FieldOffset(32)] public double firstStageVoltageThreshold;
        [FieldOffset(40)] public double maxAverageOutputPower;
        [FieldOffset(48)] public byte nStages;
        // add padding if C++ struct is larger (e.g., pad to 56 bytes)
    }

}
