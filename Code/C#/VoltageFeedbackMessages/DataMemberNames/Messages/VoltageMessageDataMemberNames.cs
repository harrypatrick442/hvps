using MessageTypes.Attributes;

namespace VoltageFeedbackMessages.DataMemberNames.Requests
{
    [MessageType(MessageTypes.Voltage)]
    public static class VoltageMessageDataMemberNames
    {
        public const string Voltage = "v";
    }
}


