using Core.Messages.Messages;
using HVPSCore.Enums;
using HVPSAPI.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class StateChangedMessage : TypedMessageBase
    {
        [JsonPropertyName(StateChangedMessageDataMemberNames.State)]
        [JsonInclude]
        [DataMember(Name = StateChangedMessageDataMemberNames.State)]
        public HVPSState State { get; protected set; }
        public StateChangedMessage()
            : base()
        {
            Type = MessageTypes.StateChanged;
        }
    }
}
