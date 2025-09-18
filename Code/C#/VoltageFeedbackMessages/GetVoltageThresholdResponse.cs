using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(GetVoltageThresholdResponseDataMemberNames))]
    public class GetVoltageThresholdResponse
    {
        private double _Voltage;
        [JsonPropertyName(GetVoltageThresholdResponseDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = GetVoltageThresholdResponseDataMemberNames.Voltage)]
        public double Voltage { get { return _Voltage; } set { _Voltage = value; } }
    }
}


