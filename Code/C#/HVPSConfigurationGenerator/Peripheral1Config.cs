using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit)]
    public struct Peripheral1Config
    {
        [FieldOffset(0)] public UInt32 idleColour;
        [FieldOffset(8)] public UInt32 liveColour;
        [FieldOffset(16)] public UInt32 runningSystemChecksColour;
        [FieldOffset(24)] public UInt32 shuttingDownColour;
        [FieldOffset(32)] public UInt32 shutDownColour;
        [FieldOffset(40)] public UInt32 errorColour;
        [FieldOffset(48)] public UInt32 unknownColour;
    }
}
