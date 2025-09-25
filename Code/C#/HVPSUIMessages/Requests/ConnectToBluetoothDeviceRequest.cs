
using Core.Messages.Messages;
using HVPSUIMessages.DataMemberNames.Requests;
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace HVPSUIMessages.Requests
{
    [DataContract]
    public class ConnectToBluetoothDeviceRequest : TicketedMessageBase
    {


        [JsonPropertyName(ConnectToBluetoothDeviceRequestDataMemberNames.Address)]
        [JsonInclude]
        [DataMember(Name = ConnectToBluetoothDeviceRequestDataMemberNames.Address)]
        public string Address { get; protected set; }
        public ConnectToBluetoothDeviceRequest(string address)
            : base(MessageTypes.ConnectToBluetoothDevice)
        {
            Address = address;
        }
        protected ConnectToBluetoothDeviceRequest()
            : base(MessageTypes.ConnectToBluetoothDevice) { }
    }
}