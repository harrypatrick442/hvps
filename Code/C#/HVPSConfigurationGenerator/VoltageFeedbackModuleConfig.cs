using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit)]
    public struct VoltageFeedbackModuleConfig
    {
        [FieldOffset(0)] public double vHvOverVadcRatio;
        [FieldOffset(8)] public double defaultThreshold;
        [FieldOffset(16)] public double broadcastFrequencyHz;
    }
}
