using MessageTypes.Attributes;

namespace VoltageFeedbackMessages.DataMemberNames.Requests
{
    [MessageType(MessageTypes.SetVoltageThreshold)]
    public static class SetVoltageThresholdRequestDataMemberNames
    {
        public const string Voltage = "v";
    }
}


    