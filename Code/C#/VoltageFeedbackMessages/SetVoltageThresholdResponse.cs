using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(SetVoltageThresholdResponseDataMemberNames))]
    public class SetVoltageThresholdResponse
    {
        private float _Voltage;
        [JsonPropertyName(SetVoltageThresholdResponseDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = SetVoltageThresholdResponseDataMemberNames.Voltage)]
        public float Voltage { get { return _Voltage; } protected set { _Voltage = value; } }
    }
}


