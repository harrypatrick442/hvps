using Core.Messages.Messages;
using HVPSMessages.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSMessages.Messages
{
    [DataContract]
    public class HVPSLiveDataMessage : TypedMessageBase
    {
        [JsonPropertyName(HVPSLiveDataMessageDataMemberNames.OutputVoltage)]
        [JsonInclude]
        [DataMember(Name = HVPSLiveDataMessageDataMemberNames.OutputVoltage)]
        public double OutputVoltage { get; protected set; }
        [JsonPropertyName(HVPSLiveDataMessageDataMemberNames.OutputCurrent)]
        [JsonInclude]
        [DataMember(Name = HVPSLiveDataMessageDataMemberNames.OutputCurrent)]
        public double OutputCurrent { get; protected set; }
        [JsonPropertyName(HVPSLiveDataMessageDataMemberNames.TotalOutputEnergy)]
        [JsonInclude]
        [DataMember(Name = HVPSLiveDataMessageDataMemberNames.TotalOutputEnergy)]
        public double TotalOutputEnergy { get; protected set; }
        [JsonPropertyName(HVPSLiveDataMessageDataMemberNames.FirstStageVoltage)]
        [JsonInclude]
        [DataMember(Name = HVPSLiveDataMessageDataMemberNames.FirstStageVoltage)]
        public double FirstStageVoltage { get; protected set; }
        [JsonPropertyName(HVPSLiveDataMessageDataMemberNames.PeakPrimaryCurrent)]
        [JsonInclude]
        [DataMember(Name = HVPSLiveDataMessageDataMemberNames.PeakPrimaryCurrent)]
        public double PeakPrimaryCurrent { get; protected set; }
        public HVPSLiveDataMessage()
            : base()
        {
            Type = MessageTypes.HVPSUILiveData;
        }
    }
}
