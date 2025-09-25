using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.LiveData)]
    public static class LiveDataMessageDataMemberNames
    {
        public const string OutputVoltage = "v";
        public const string OutputCurrent = "c";
        public const string TotalOutputEnergy = "t";
        public const string FirstStageVoltage = "f";
        public const string PeakPrimaryCurrent = "p";
    }
}
