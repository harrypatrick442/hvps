using Bluetooth;
using Bluetooth.Messages;
using Core.Events;
using Core.Interfaces;
using HVPSUIMessages.Requests;
using HVPSUIMessages.Responses;
using Native.Messages;
using Native.Messaging;
using Native.WebViewInterface;
using HVPSMessages.Messages;

namespace HVPSUI
{
    public class MessageHandler
    {
        private WebViewMessagingInterface _WebViewMessagingInterface;
        private RegistrationMessageHandler _DeviceRegistrationMessageHandler;
        private ConnectedBluetoothDeviceHandle? _ConnectedBluetoothDeviceHandle;
        private PingDisconnectDetector _PingDisconnectDetector;
        private readonly object _LockObjectConnectedBluetoothDeviceHandle = new object();
        public MessageHandler(WebViewMessagingInterface webViewMessagingInterface) {
            _WebViewMessagingInterface = webViewMessagingInterface;
            _DeviceRegistrationMessageHandler = new RegistrationMessageHandler(null);
            _PingDisconnectDetector = new PingDisconnectDetector(_DeviceRegistrationMessageHandler, DeviceDisconnected);
            _WebViewMessagingInterface.OnMessage += HandleMessageFromJavaScript;
            _WebViewMessagingInterface.RegisterMethod<
                GetAvailableBluetoothDevicesRequest, GetAvailableBluetoothDevicesResponse>(
                HVPSUIMessages.MessageTypes.GetAvaialbleBluetoothDevices, HandleGetAvailableBluetoothDevices);
            _WebViewMessagingInterface.RegisterMethod<
                ConnectToBluetoothDeviceRequest, ConnectToBluetoothDeviceResponse>(
                HVPSUIMessages.MessageTypes.ConnectToBluetoothDevice, HandleConnectToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                HVPSStartMessage>(
                HVPSMessages.MessageTypes.HVPSUIStart, HandleHVPSStartMessage);
            _WebViewMessagingInterface.RegisterMethod<
                HVPSStopMessage>(
                HVPSMessages.MessageTypes.HVPSUIStop, HandleHVPSStopMessage);
            _WebViewMessagingInterface.RegisterMethod<
                HVPSShutDownMessage>(
                HVPSMessages.MessageTypes.HVPSUIShutDown, HandleHVPSShutDownMessage);
            _WebViewMessagingInterface.RegisterMethod<
                HVPSRunSystemChecksOnlyMessage>(
                HVPSMessages.MessageTypes.HVPSUIRunSystemChecksOnly, HandleHVPSRunSystemChecksOnlyMessage);
            _DeviceRegistrationMessageHandler.RegisterMethod<
                ConsoleMessage>(
                    Native.MessageTypes.ConsoleMessage,
                    HandleConsoleMessage
                );
            _DeviceRegistrationMessageHandler.OnMessage += (o, e) => _PingDisconnectDetector.Received();
        }
        private void HandleHVPSStartMessage(
            HVPSStartMessage message)
        {
            SendToBluetoothDevice(new HVPSAPI.Messages.StartMessage());
        }
        private void HandleHVPSStopMessage(
            HVPSStopMessage message)
        {
            SendToBluetoothDevice(new HVPSAPI.Messages.StopMessage());
        }
        private void HandleHVPSShutDownMessage(
            HVPSShutDownMessage message)  
        {
            SendToBluetoothDevice(new HVPSAPI.Messages.ShutDownMessage());
        }
        private void HandleHVPSRunSystemChecksOnlyMessage(
            HVPSRunSystemChecksOnlyMessage message)
        {
            SendToBluetoothDevice(new HVPSAPI.Messages.RunSystemChecksOnlyMessage());
        }
        private void HandleConsoleMessage(
            ConsoleMessage message)
        {
            _WebViewMessagingInterface.Send(message);
        }
        private void HandleMessageFromJavaScript(object sender, TypedMessageEventArgs e) {
            //Logs.Default.Info(e.Message);
        }
        private GetAvailableBluetoothDevicesResponse HandleGetAvailableBluetoothDevices(
            GetAvailableBluetoothDevicesRequest request)
        {
            try
            {
                var devices = BluetoothHelper.Scan(MatchBluetoothDevice);
                return GetAvailableBluetoothDevicesResponse.Success(
                    devices, request.Ticket);
            }
            catch (BluetoothException ex)
            {
                return GetAvailableBluetoothDevicesResponse.Failure(
                    ex.FailedReason, request.Ticket);
            }
            catch (Exception ex)
            {
                return GetAvailableBluetoothDevicesResponse.Failure(BluetoothFailedReason.Unknown, request.Ticket);
            }
        }
        private ConnectToBluetoothDeviceResponse HandleConnectToBluetoothDevice(
            ConnectToBluetoothDeviceRequest request)
        {
            lock (_LockObjectConnectedBluetoothDeviceHandle)
            {
                _ConnectedBluetoothDeviceHandle?.Dispose();
                _ConnectedBluetoothDeviceHandle = null;
                _PingDisconnectDetector.Stop();
                if (string.IsNullOrEmpty(request.Address))
                {
                    return ConnectToBluetoothDeviceResponse.Success(request.Address, request.Ticket);
                }
                try
                {
                    _ConnectedBluetoothDeviceHandle = BluetoothHelper.Connect(
                        request.Address, () => _DeviceRegistrationMessageHandler);
                    _PingDisconnectDetector.StartClean();
                    return ConnectToBluetoothDeviceResponse.Success(request.Address, request.Ticket);
                }
                catch (BluetoothException ex)
                {
                    return ConnectToBluetoothDeviceResponse.Failure(
                        request.Address, ex.FailedReason, request.Ticket);
                }
                catch (Exception ex)
                {
                    return ConnectToBluetoothDeviceResponse.Failure(
                        request.Address, BluetoothFailedReason.Unknown, request.Ticket);
                }
            }
        }
        private void SendToBluetoothDevice<TMessage>(TMessage message)
        where TMessage:ITypedMessage{
            _DeviceRegistrationMessageHandler.Send(message);
        }
        private static bool MatchBluetoothDevice(BluetoothDevice bluetoothDeviceInfo)
        {
            return bluetoothDeviceInfo.Name.Contains("HVPS");
        }
        private void DeviceDisconnected() {
            lock (_LockObjectConnectedBluetoothDeviceHandle)
            {
                _PingDisconnectDetector.Stop();
                _ConnectedBluetoothDeviceHandle?.Dispose();
                _ConnectedBluetoothDeviceHandle = null;
            }
            SendDisconnectedToUI();
        }
        private void SendDisconnectedToUI() {
            _WebViewMessagingInterface.Send(new BluetoothDeviceDisconnectedMessage());
        }
    }
}
