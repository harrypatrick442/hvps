using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class Error : TypedMessageBase
    {
        [JsonPropertyName(ErrorDataMemberNames.Message)]
        [JsonInclude]
        [DataMember(Name = ErrorDataMemberNames.Message)]
        public string Message { get; protected set; }
        public Error()
            : base()
        {
            Type = MessageTypes.Error;
        }
    }
}
