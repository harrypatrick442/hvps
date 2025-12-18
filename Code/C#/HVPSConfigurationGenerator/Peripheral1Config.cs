using System.Runtime.InteropServices;
namespace HVPSConfigurationGenerator
{
    [StructLayout(LayoutKind.Explicit)]
    public struct Peripheral1Configuration
    {
        [FieldOffset(0)] public UInt32 idleColour;
        [FieldOffset(4)] public UInt32 liveColour;
        [FieldOffset(8)] public UInt32 runningSystemChecksColour;
        [FieldOffset(12)] public UInt32 shuttingDownColour;
        [FieldOffset(16)] public UInt32 shutDownColour;
        [FieldOffset(20)] public UInt32 errorColour;
        [FieldOffset(24)] public UInt32 unknownColour;
        [FieldOffset(28)] public UInt32 idleFlashDelayMs;
        [FieldOffset(32)] public UInt32 liveFlashDelayMs;
        [FieldOffset(36)] public UInt32 runningSystemChecksFlashDelayMs;
        [FieldOffset(40)] public UInt32 shuttingDownFlashDelayMs;
        [FieldOffset(44)] public UInt32 shutDownFlashDelayMs;
        [FieldOffset(48)] public UInt32 errorFlashDelayMs;
        [FieldOffset(52)] public UInt32 unknownFlashDelayMs;
    }
}
