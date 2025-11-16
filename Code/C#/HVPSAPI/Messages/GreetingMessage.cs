using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSAPI.DataMemberNames.Responses;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class GreetingMessage : TypedMessageBase
    {
        [JsonPropertyName(GreetingMessageDataMemberNames.LastAbortMessage)]
        [JsonInclude]
        [DataMember(Name = GreetingMessageDataMemberNames.LastAbortMessage)]
        public LastAbortMessage? LastAbortMessage { get; set; }
        [JsonPropertyName(GreetingMessageDataMemberNames.CoreDumpSummaryMessage)]
        [JsonInclude]
        [DataMember(Name = GreetingMessageDataMemberNames.CoreDumpSummaryMessage)]
        public CoreDumpSummaryMessage? CoreDumpSummaryMessage { get; set; }
        public GreetingMessage()
            : base()
        {
            Type = MessageTypes.Greeting;
        }
    }
}
