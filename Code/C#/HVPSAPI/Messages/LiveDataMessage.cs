using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class LiveDataMessage : TypedMessageBase
    {
        [JsonPropertyName(LiveDataMessageDataMemberNames.OutputVoltage)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.OutputVoltage)]
        public float OutputVoltage { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.OutputCurrent)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.OutputCurrent)]
        public float OutputCurrent { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.TotalOutputEnergy)]
        [JsonInclude]   
        [DataMember(Name = LiveDataMessageDataMemberNames.TotalOutputEnergy)]
        public float TotalOutputEnergy { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.FirstStageVoltage)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.FirstStageVoltage)]
        public float FirstStageVoltage { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.Frequency)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.Frequency)]
        public UInt32? Frquency { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.PeakPrimaryCurrent)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.PeakPrimaryCurrent)]
        public float PeakPrimaryCurrent { get; protected set; }
        public LiveDataMessage()
            : base()
        {
            Type = MessageTypes.LiveData;
        }
    }
}
