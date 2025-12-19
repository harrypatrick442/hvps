using MessageTypes.Attributes;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using VoltageFeedbackMessages.DataMemberNames.Requests;
using VoltageFeedbackMessages.DataMemberNames.Responses;

namespace VoltageFeedbackMessages.Messages
{
    [DataMemberNamesClass(typeof(VoltageMessageDataMemberNames))]
    public class VoltageMessage
    {
        private float _Voltage;
        [JsonPropertyName(VoltageMessageDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = VoltageMessageDataMemberNames.Voltage)]
        public float Voltage { get { return _Voltage; } protected set { _Voltage = value; } }
    }
}


