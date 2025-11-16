using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Messages
{
    [MessageType(MessageTypes.Greeting)]
    public static class GreetingMessageDataMemberNames
    {
        [DataMemberNamesClass(typeof(LastAbortMessageDataMemberNames))]
        public const string LastAbortMessage = "la";
        [DataMemberNamesClass(typeof(CoreDumpSummaryMessageDataMemberNames))]
        public const string CoreDumpSummaryMessage = "cd";
    }
}
