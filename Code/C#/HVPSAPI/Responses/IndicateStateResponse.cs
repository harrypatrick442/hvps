using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSAPI.DataMemberNames.Responses;
using HVPSAPI.Messages;
using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
namespace HVPSAPI.Responses
{
    [DataMemberNamesClass(typeof(IndicateStateResponseDataMemberNames))]
    public class IndicateStateResponse
    {
        [JsonPropertyName(IndicateStateResponseDataMemberNames.Success)]
        [JsonInclude]
        [DataMember(Name = IndicateStateResponseDataMemberNames.Success)]
        public bool Success{ get; protected set; }
    }
}


