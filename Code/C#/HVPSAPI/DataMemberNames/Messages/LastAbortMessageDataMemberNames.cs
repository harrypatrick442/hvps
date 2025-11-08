using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.CoreDumpSummary)]
    public static class LastAbortMessageDataMemberNames
    {
        public const string Reason = "r";
    }
}
