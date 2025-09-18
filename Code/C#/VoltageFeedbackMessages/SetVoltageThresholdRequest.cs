using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;

namespace VoltageFeedbackMessages.Requests
{
    [DataMemberNamesClass(typeof(SetVoltageThresholdRequestDataMemberNames))]
    public class SetVoltageThresholdRequest
    {
        private double _Voltage;
        [JsonPropertyName(SetVoltageThresholdRequestDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = SetVoltageThresholdRequestDataMemberNames.Voltage)]
        public double Voltage { get { return _Voltage; } set { _Voltage = value; } }
    }
}


