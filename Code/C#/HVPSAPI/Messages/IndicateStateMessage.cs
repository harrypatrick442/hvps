using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSCore.Enums;
using Microsoft.Win32;
using System;
using System.Net;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using HVPSCore.Enums;
using MessageTypes.Attributes;
using HVPSAPI.DataMemberNames.Requests;
namespace HVPSAPI.Messages
{
    [DataContract]
    public class IndicateStateMessage : TypedMessageBase
    {
        [JsonPropertyName(IndicateStateMessageDataMemberNames.SystemState)]
        [JsonInclude]
        [DataMember(Name = IndicateStateMessageDataMemberNames.SystemState)]
        public SystemState SystemState { get; protected set; }
        public IndicateStateMessage()
            : base()
        {
            Type = MessageTypes.IndicateState;
        }
    }
}
