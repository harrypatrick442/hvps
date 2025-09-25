using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.Stop)]
    public static class StopMessageRequestDataMemberNames
    {
        public const string RequestForDevice = "a";
    }
}
