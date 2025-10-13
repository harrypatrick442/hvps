using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit)]
    public struct CurrentFeedbackModuleConfig
    {
        [FieldOffset(0)] public double senseResistanceOhms;
        [FieldOffset(8)] public double outputCurrentLimitingResistanceOhms;
        [FieldOffset(16)] public double cumulativeEnergyThresholdJ;
        [FieldOffset(16)] public double energyDisipatedJPerS;
        [FieldOffset(16)] public double broadcastFrequencyHz;
    }
}
