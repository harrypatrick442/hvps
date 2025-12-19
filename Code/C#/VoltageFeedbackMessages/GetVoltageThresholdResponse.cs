using HVPSAPI.DataMemberNames.Responses;
using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(GetVoltageThresholdResponseDataMemberNames))]
    public class GetVoltageThresholdResponse
    {
        private float _Voltage;
        [JsonPropertyName(GetVoltageThresholdResponseDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = GetVoltageThresholdResponseDataMemberNames.Voltage)]
        public float Voltage { get { return _Voltage; } protected set { _Voltage = value; } }
    }
}


