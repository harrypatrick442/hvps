using MessageTypes.Attributes;

namespace VoltageFeedbackMessages.DataMemberNames.Requests
{
    [MessageType(MessageTypes.GetVoltage)]
    public static class GetVoltageRequestDataMemberNames
    {
        public const string Voltage = "v";
    }
}


