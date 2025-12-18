using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit)]
    public struct VoltageFeedbackModuleConfiguration
    {
        [FieldOffset(0)] public float broadcastFrequencyHz;
        [FieldOffset(4)] public float  defaultThreshold;
        [FieldOffset(8)] public float vHvOverVadcRatio;
    }
}
