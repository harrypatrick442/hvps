using Core.Messages.Messages;
using HVPSCore.Enums;
using HVPSMessages.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSStateChangedMessage : TypedMessageBase
    {
        [JsonPropertyName(HVPSStateChangedMessageDataMemberNames.State)]
        [JsonInclude]
        [DataMember(Name = HVPSStateChangedMessageDataMemberNames.State)]
        public HVPSState State { get; protected set; }
        public HVPSStateChangedMessage()
            : base()
        {
            Type = MessageTypes.HVPSUIStateChanged;
        }
    }
}
