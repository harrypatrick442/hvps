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
    public class CoreDumpMessage : TypedMessageBase
    {

        [JsonPropertyName(CoreDumpMessageDataMemberNames.ARegisterSetWhenTheExceptionCaused)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.ARegisterSetWhenTheExceptionCaused)]
        public UInt32[] ARegisterSetWhenTheExceptionCaused { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.Backtrace)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.Backtrace)]
        public UInt32[] Backtrace { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.BitMaskOfAvailableEPCxRegisters)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.BitMaskOfAvailableEPCxRegisters)]
        //8 bits
        public Int16 BitMaskOfAvailableEPCxRegisters { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.CauseOfException)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.CauseOfException)]
        public UInt32 CauseOfException { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.CrashingApplicationsSHA256SumAsAString)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.CrashingApplicationsSHA256SumAsAString)]
        public string CrashingApplicationsSHA256SumAsAString { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.PCRegisterAddressAtExceptionLevel1To7)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.PCRegisterAddressAtExceptionLevel1To7)]
        public UInt32[] PCRegisterAddressAtExceptionLevel1To7 { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.ProgramCounterForException)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.ProgramCounterForException)]
        public UInt32 ProgramCounterForException { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.TaskName)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.TaskName)]
        public string TaskName{ get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.TaskPointer)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.TaskPointer)]
        public UInt32 TaskPointer { get; protected set; }
        [JsonPropertyName(CoreDumpMessageDataMemberNames.Version)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.Version)]
        public UInt32 Version { get;protected set; }

        [JsonPropertyName(CoreDumpMessageDataMemberNames.VirtualAddressOfException)]
        [JsonInclude]
        [DataMember(Name = CoreDumpMessageDataMemberNames.VirtualAddressOfException)]
        public UInt32 VirtualAddressOfException { get; protected set; }
        public CoreDumpMessage()
            : base()
        {
            Type = MessageTypes.CoreDump;
        }
    }
}
