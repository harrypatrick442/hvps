using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class LiveDataMessage : TypedMessageBase
    {
        [JsonPropertyName(LiveDataMessageDataMemberNames.OutputVoltageVolts)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.OutputVoltageVolts)]
        public float OutputVoltageVolts { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.OutputVoltageValueBoundType)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.OutputVoltageValueBoundType)]
        public byte OutputVoltageValueBoundType { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.OutputCurrentAmps)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.OutputCurrentAmps)]
        public float OutputCurrentAmps { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.TotalOutputEnergyJouls)]
        [JsonInclude]   
        [DataMember(Name = LiveDataMessageDataMemberNames.TotalOutputEnergyJouls)]
        public float TotalOutputEnergyJouls { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.FirstStageVoltageValueBoundType)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.FirstStageVoltageValueBoundType)]
        public byte FirstStageVoltageValueBoundType { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.FirstStageVoltageVolts)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.FirstStageVoltageVolts)]
        public float FirstStageVoltageVolts { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.FrequencyHz)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.FrequencyHz)]
        public float FrquencyHz { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.FrequencyHzValueBoundType)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.FrequencyHzValueBoundType)]
        public byte FrequencyHzValueBoundType { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.PeakPrimaryCurrentAmps)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.PeakPrimaryCurrentAmps)]
        public float PeakPrimaryCurrentAmps { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.PeakPrimaryCurrentValueBoundType)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.PeakPrimaryCurrentValueBoundType)]
        public byte PeakPrimaryCurrentValueBoundType { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.PrimaryPowerWatts)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.PrimaryPowerWatts)]
        public float PrimaryPowerWatts { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.TotalPrimaryEnergyJouls)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.TotalPrimaryEnergyJouls)]
        public float TotalPrimaryEnergyJouls { get; protected set; }
        [JsonPropertyName(LiveDataMessageDataMemberNames.MosfetTemperatureDegreesC)]
        [JsonInclude]
        [DataMember(Name = LiveDataMessageDataMemberNames.MosfetTemperatureDegreesC)]
        public float MosfetTemperatureDegreesC { get; protected set; }
        public LiveDataMessage()
            : base()
        {
            Type = MessageTypes.LiveData;
        }
    }
}
