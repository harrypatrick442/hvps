using MessageTypes.Attributes;

namespace HVPSMessages.DataMemberNames.Messages
{
    [MessageType(MessageTypes.HVPSUILiveData)]
    public static class HVPSLiveDataMessageDataMemberNames
    {
        public const string OutputVoltage = "ov";
        public const string OutputCurrent = "oc";
        public const string TotalOutputEnergy = "toe";
        public const string FirstStageVoltage = "fsv";
        public const string PeakPrimaryCurrent = "pc";
    }
}
