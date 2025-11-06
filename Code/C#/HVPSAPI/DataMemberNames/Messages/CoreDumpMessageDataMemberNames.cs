using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.CoreDump)]
    public static class CoreDumpMessageDataMemberNames
    {
        public const string TaskName = "tn";
        public const string TaskPointer = "tp";
        public const string ProgramCounterForException = "pc";
        public const string VirtualAddressOfException = "va";
        public const string Version = "v";
        public const string CrashingApplicationsSHA256SumAsAString = "casha";
        public const string Backtrace = "b";
        public const string CauseOfException = "coe";
        public const string ARegisterSetWhenTheExceptionCaused = "areg";
        public const string PCRegisterAddressAtExceptionLevel1To7 = "pcreg";
        public const string BitMaskOfAvailableEPCxRegisters = "bitm";
    }
}
