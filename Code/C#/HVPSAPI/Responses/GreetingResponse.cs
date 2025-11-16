using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSAPI.DataMemberNames.Responses;
using HVPSAPI.Messages;
using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
namespace HVPSAPI.Responses
{
    [DataMemberNamesClass(typeof(GreetingResponseDataMemberNames))]
    public class GreetingResponse
    {
        [JsonPropertyName(GreetingResponseDataMemberNames.LastAbortMessage)]
        [JsonInclude]
        [DataMember(Name = GreetingResponseDataMemberNames.LastAbortMessage)]
        public LastAbortMessage? LastAbortMessage { get; set; }
        [JsonPropertyName(GreetingResponseDataMemberNames.CoreDumpSummaryMessage)]
        [JsonInclude]
        [DataMember(Name = GreetingResponseDataMemberNames.CoreDumpSummaryMessage)]
        public CoreDumpSummaryMessage? CoreDumpSummaryMessage { get; set; }
    }
}


