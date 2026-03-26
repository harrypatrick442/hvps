using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.LiveData)]
    public static class LiveDataMessageDataMemberNames
    {
        public const string OutputVoltageVolts = "a";
        public const string OutputVoltageValueBoundType = "b";
        public const string OutputCurrentAmps = "c";
        public const string TotalOutputEnergyJouls = "d";
        public const string FirstStageVoltageVolts = "e";
        public const string FirstStageVoltageValueBoundType = "f";
        public const string FrequencyHz = "g";
        public const string FrequencyHzValueBoundType = "h";
        public const string PeakPrimaryCurrentAmps = "i";
        public const string PeakPrimaryCurrentValueBoundType = "l";
        public const string PrimaryPowerWatts = "j";
        public const string TotalPrimaryEnergyJouls = "k";
        public const string MosfetTemperatureDegreesC = "m";
    }
}
