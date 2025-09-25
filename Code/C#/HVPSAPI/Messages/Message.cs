using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class Message : TypedMessageBase
    {
        [JsonPropertyName(MessageDataMemberNames.Content)]
        [JsonInclude]
        [DataMember(Name = MessageDataMemberNames.Content)]
        public string Content { get; protected set; }
        public Message()
            : base()
        {
            Type = MessageTypes.Message;
        }
    }
}
