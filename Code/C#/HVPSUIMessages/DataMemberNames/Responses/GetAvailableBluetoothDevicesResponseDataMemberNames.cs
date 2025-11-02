using Bluetooth.DataMemberNames.Messages;
using MessageTypes.Attributes;

namespace HVPSUIMessages.DataMemberNames.Responses
{
    public static class GetAvailableBluetoothDevicesResponseDataMemberNames
    {
        [DataMemberNamesClass(typeof(BluetoothDeviceDataMemberNames), isArray:true)]
        public const string Devices = "d";
        public const string FailedReason = "s";
    }
}
