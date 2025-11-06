using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSCore.Enums;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class ErrorMessage : TypedMessageBase
    {

        [JsonPropertyName(ErrorMessageDataMemberNames.ErrorType)]
        [JsonInclude]
        [DataMember(Name = ErrorMessageDataMemberNames.ErrorType)]
        public ErrorType ErrorType { get; protected set; }

        [JsonPropertyName(ErrorMessageDataMemberNames.SerializedError)]
        [JsonInclude]
        [DataMember(Name = ErrorMessageDataMemberNames.SerializedError)]
        public string SerializedError { get; protected set; }
        public ErrorMessage()
            : base()
        {
            Type = MessageTypes.Error;
        }
    }
}
