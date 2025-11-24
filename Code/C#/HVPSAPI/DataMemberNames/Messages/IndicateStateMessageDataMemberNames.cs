using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.IndicateState)]
    public static class IndicateStateMessageDataMemberNames
    {
        public const string SystemState = "s";
    }
}
