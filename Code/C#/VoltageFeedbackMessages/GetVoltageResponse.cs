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
        private float _Voltage;
        [JsonPropertyName(GetVoltageResponseDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = GetVoltageResponseDataMemberNames.Voltage)]
        public float Voltage { get { return _Voltage; } protected set { _Voltage = value; } }
    }
}


