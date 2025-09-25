using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.StateChanged)]
    public static class StateChangedMessageDataMemberNames
    {
        public const string State = "s";
    }
}
