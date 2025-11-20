using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.CoreDumpSummary)]
    public static class CoreDumpSummaryMessageDataMemberNames
    {
        public const string SubsystemIdentifier = "si";
        public const string TaskName = "tn";
        public const string TaskPointer = "tp";
        public const string ProgramCounterForException = "pc";
        public const string VirtualAddressOfException = "va";
        public const string Version = "v";
        public const string CrashingApplicationsSHA256SumAsAString = "casha";
        public const string Backtrace = "b";
        public const string BacktraceCorrupted = "bcpt";
        public const string CauseOfException = "coe";
        public const string ARegisterSetWhenTheExceptionCaused = "areg";
        public const string PCRegisterAddressAtExceptionLevel1To7 = "pcreg";
        public const string BitMaskOfAvailableEPCxRegisters = "bitm";
    }
}
