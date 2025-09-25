using MessageTypes.Attributes;

namespace HVPSMessages.DataMemberNames.Messages
{
    [MessageType(MessageTypes.HVPSUIStateChanged)]
    public static class HVPSStateChangedMessageDataMemberNames
    {
        public const string State = "s";
    }
}
