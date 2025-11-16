using HVPSAPI.DataMemberNames.Messages;
using MessageTypes.Attributes;

namespace HVPSAPI.DataMemberNames.Responses
{
    public static class GreetingResponseDataMemberNames
    {
        [DataMemberNamesClass(typeof(LastAbortMessageDataMemberNames))]
        public const string LastAbortMessage = "la";
        [DataMemberNamesClass(typeof(CoreDumpSummaryMessageDataMemberNames))]
        public const string CoreDumpSummaryMessage = "cd";
    }
}
