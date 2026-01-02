using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(GetVoltageResponseDataMemberNames))]
    public class GetVoltageResponse
    {
        [JsonPropertyName(GetVoltageResponseDataMemberNames.RawVoltage)]
        [JsonInclude]
        [DataMember(Name = GetVoltageResponseDataMemberNames.RawVoltage)]
        public UInt16 RawVoltage { get; protected set; }
        [JsonPropertyName(GetVoltageResponseDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = GetVoltageResponseDataMemberNames.Voltage)]
        public float Voltage { get; protected set; }
    }
}


