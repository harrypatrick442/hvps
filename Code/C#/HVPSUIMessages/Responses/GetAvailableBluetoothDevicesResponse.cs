using Bluetooth;
using Bluetooth.Messages;
using Core.Messages.Responses;
using HVPSUIMessages.DataMemberNames.Responses;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSUIMessages.Responses
{
    [DataContract]
    public class GetAvailableBluetoothDevicesResponse : TicketedResponseMessageBase
    {
        [JsonPropertyName(GetAvailableBluetoothDevicesResponseDataMemberNames.Devices)]
        [JsonInclude]
        [DataMember(Name = GetAvailableBluetoothDevicesResponseDataMemberNames.Devices)]
        public BluetoothDevice[]? Devices { get; protected set; }
        [JsonPropertyName(GetAvailableBluetoothDevicesResponseDataMemberNames.FailedReason)]
        [JsonInclude]
        [DataMember(Name = GetAvailableBluetoothDevicesResponseDataMemberNames.FailedReason)]
        public BluetoothFailedReason? FailedReason { get; protected set; }
        public GetAvailableBluetoothDevicesResponse(
            BluetoothDevice[]? devices, BluetoothFailedReason? failedReason,
            long ticket)
            : base(ticket)
        {
            Devices = devices;
            FailedReason = failedReason;
        }
        protected GetAvailableBluetoothDevicesResponse() : base(0) { }
        public static GetAvailableBluetoothDevicesResponse Success(
            BluetoothDevice[] devices, long ticket)
        {
            return new GetAvailableBluetoothDevicesResponse(devices, null, ticket);
        }
        public static GetAvailableBluetoothDevicesResponse Failure(
            BluetoothFailedReason? failedReason, long ticket)
        {
            return new GetAvailableBluetoothDevicesResponse(null, failedReason, ticket);
        }
    }
}