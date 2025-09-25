using Bluetooth;
using Core.Messages.Responses;
using HVPSUIMessages.DataMemberNames.Responses;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSUIMessages.Responses
{
    [DataContract]
    public class ConnectToBluetoothDeviceResponse : TicketedResponseMessageBase
    {
        [JsonPropertyName(ConnectToBluetoothDeviceResponseDataMemberNames.FailedReason)]
        [JsonInclude]
        [DataMember(Name = ConnectToBluetoothDeviceResponseDataMemberNames.FailedReason)]
        public BluetoothFailedReason? FailedReason { get; protected set; }
        [JsonPropertyName(ConnectToBluetoothDeviceResponseDataMemberNames.Address)]
        [JsonInclude]
        [DataMember(Name = ConnectToBluetoothDeviceResponseDataMemberNames.Address)]
        public string? Address { get; protected set; }
        protected ConnectToBluetoothDeviceResponse(string? address, BluetoothFailedReason? failedReason, long ticket)
            : base(ticket)
        {
            Address = address;
            FailedReason = failedReason;
        }
        protected ConnectToBluetoothDeviceResponse() : base(-1) { }
        public static ConnectToBluetoothDeviceResponse Success(string address, long ticket)
        {
            return new ConnectToBluetoothDeviceResponse(address, null, ticket);
        }
        public static ConnectToBluetoothDeviceResponse Failure(string address, BluetoothFailedReason failedReason, long ticket)
        {
            return new ConnectToBluetoothDeviceResponse(address, failedReason, ticket);
        }
    }
}
