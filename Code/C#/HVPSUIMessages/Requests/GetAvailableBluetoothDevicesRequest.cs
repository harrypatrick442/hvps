using Core.Messages.Messages;
using System.Runtime.Serialization;

namespace HVPSUIMessages.Requests
{
    [DataContract]
    public class GetAvailableBluetoothDevicesRequest : TicketedMessageBase
    {

        public GetAvailableBluetoothDevicesRequest()
            : base(MessageTypes.GetAvaialbleBluetoothDevices)
        {

        }
    }
}