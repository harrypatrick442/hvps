using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.LastAbort)]
    public static class LastAbortMessageDataMemberNames
    {
        public const string SubsystemIdentifier = "si";
        public const string Reason = "r";
        public const string Backtrace = "bt";
    }
}
