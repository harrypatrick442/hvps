using Core.Messages.Messages;
using HVPSAPI.DataMemberNames.Messages;
using HVPSCore.Enums;
using Microsoft.Win32;
using System;
using System.Net;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;
using HVPSCore.Enums;
namespace HVPSAPI.Messages
{
    [DataContract]
    public class CoreDumpSummaryMessage : TypedMessageBase
    {

        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.SubsystemIdentifier)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.SubsystemIdentifier)]
        /*!< a register set when the exception caused */
        public SubsystemIdentifier SubsystemIdentifier{ get; protected set; }

        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.ARegisterSetWhenTheExceptionCaused)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.ARegisterSetWhenTheExceptionCaused)]
        /*!< a register set when the exception caused */
        public UInt32[] ARegisterSetWhenTheExceptionCaused { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.Backtrace)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.Backtrace)]
        /*!< Backtrace (array of PC) */
        public UInt32[] Backtrace { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.BacktraceCorrupted)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.BacktraceCorrupted)]
        /*!< Status flag for backtrace is corrupt or not */
        public bool BacktraceCorrupted { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.BitMaskOfAvailableEPCxRegisters)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.BitMaskOfAvailableEPCxRegisters)]
        /*!< Bit mask of available EPCx registers */
        //8 bits
        public byte BitMaskOfAvailableEPCxRegisters { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.CauseOfException)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.CauseOfException)]
        /*!< Cause of exception */
        public UInt32 CauseOfException { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.CrashingApplicationsSHA256SumAsAString)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.CrashingApplicationsSHA256SumAsAString)]
        /*!< Crashing application's SHA256 sum as a string */
        public string CrashingApplicationsSHA256SumAsAString { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.PCRegisterAddressAtExceptionLevel1To7)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.PCRegisterAddressAtExceptionLevel1To7)]
        /*!< PC register address at exception level(1 to 7) */
        public UInt32[] PCRegisterAddressAtExceptionLevel1To7 { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.ProgramCounterForException)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.ProgramCounterForException)]
        /*!< Program counter for exception */
        public UInt32 ProgramCounterForException { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.TaskName)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.TaskName)]
        /*!< Name of the task that caused exception */
        public string TaskName{ get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.TaskPointer)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.TaskPointer)]
        /*!< TCB pointer to the task causing exception */
        public UInt32 TaskPointer { get; protected set; }
        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.Version)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.Version)]
        /*!< Core dump version */
        public UInt32 Version { get;protected set; }

        [JsonPropertyName(CoreDumpSummaryMessageDataMemberNames.VirtualAddressOfException)]
        [JsonInclude]
        [DataMember(Name = CoreDumpSummaryMessageDataMemberNames.VirtualAddressOfException)]
        /*!< Virtual address of exception */
        public UInt32 VirtualAddressOfException { get; protected set; }
        public CoreDumpSummaryMessage()
            : base()
        {
            Type = MessageTypes.CoreDumpSummary;
        }
    }
}
