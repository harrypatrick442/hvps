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
        [JsonPropertyName(VoltageMessageDataMemberNames.Voltage)]
        [JsonInclude]
        [DataMember(Name = VoltageMessageDataMemberNames.Voltage)]
        public float Voltage { get; protected set; }

        [JsonPropertyName(VoltageMessageDataMemberNames.RawVoltage)]
        [JsonInclude]
        [DataMember(Name = VoltageMessageDataMemberNames.RawVoltage)]
        public UInt16 RawVoltage { get; protected set; }
    }
}


