using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSCore.Enums;
using Microsoft.Win32;
using System;
using System.Net;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSAPI.Messages
{
    [DataContract]
    public class LastAbortMessage : TypedMessageBase
    {
        [JsonPropertyName(LastAbortMessageDataMemberNames.SubsystemIdentifier)]
        [JsonInclude]
        [DataMember(Name = LastAbortMessageDataMemberNames.SubsystemIdentifier)]
        /*!< a register set when the exception caused */
        public SubsystemIdentifier SubsystemIdentifier { get; protected set; }
        [JsonPropertyName(LastAbortMessageDataMemberNames.Backtrace)]
        [JsonInclude]
        [DataMember(Name = LastAbortMessageDataMemberNames.Backtrace)]
        public UInt32[] Backtrace { get; protected set; }
        [JsonPropertyName(LastAbortMessageDataMemberNames.Reason)]
        [JsonInclude]
        [DataMember(Name = LastAbortMessageDataMemberNames.Reason)]
        public string Reason { get; protected set; }
        public LastAbortMessage()
            : base()
        {
            Type = MessageTypes.LastAbort;
        }
    }
}
