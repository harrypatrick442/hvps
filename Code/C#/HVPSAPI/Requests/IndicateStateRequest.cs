using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Requests;
using HVPSCore.Enums;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Requests
{
    [DataContract]
    public class IndicateStateRequest : TypedMessageBase
    {
        [JsonPropertyName(IndicateStateRequestDataMemberNames.SystemState)]
        [JsonInclude]
        [DataMember(Name = IndicateStateRequestDataMemberNames.SystemState)]
        public SystemState SystemState{ get; protected set; }
        public IndicateStateRequest()
            : base()
        {
            Type = MessageTypes.IndicateStateRequest;
        }
    }
}
