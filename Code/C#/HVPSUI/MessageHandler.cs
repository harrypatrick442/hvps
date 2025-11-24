using Bluetooth;
using Bluetooth.Messages;
using Core.Events;
using Core.Interfaces;
using HVPSUIMessages.Requests;
using HVPSUIMessages.Responses;
using Native.Messages;
using Native.Messaging;
using Native.WebViewInterface;
using HVPSAPI.Messages;

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
            _PingDisconnectDetector = new PingDisconnectDetector(
                _DeviceRegistrationMessageHandler,
                DeviceDisconnected,
                HVPSConstants.Constants.PingTimeoutMilliseconds
            );
            _WebViewMessagingInterface.OnMessage += HandleMessageFromJavaScript;
            _WebViewMessagingInterface.RegisterMethod<
                GetAvailableBluetoothDevicesRequest, GetAvailableBluetoothDevicesResponse>(
                HVPSUIMessages.MessageTypes.GetAvaialbleBluetoothDevices, HandleGetAvailableBluetoothDevices);
            _WebViewMessagingInterface.RegisterMethod<
                ConnectToBluetoothDeviceRequest, ConnectToBluetoothDeviceResponse>(
                HVPSUIMessages.MessageTypes.ConnectToBluetoothDevice, HandleConnectToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                StartMessage>(
                HVPSAPI.MessageTypes.Start, SendToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                StopMessage>(
                HVPSAPI.MessageTypes.Stop, SendToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                ShutDownMessage>(
                HVPSAPI.MessageTypes.ShutDown, SendToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                RunSystemChecksOnlyMessage>(
                HVPSAPI.MessageTypes.RunSystemChecksOnly,
                SendToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                TestMessage>(
                HVPSAPI.MessageTypes.Test,
                SendToBluetoothDevice);
            _WebViewMessagingInterface.RegisterMethod<
                ClearLoggedErrorsMessage>(
                HVPSAPI.MessageTypes.ClearLoggedErrors,
                SendToBluetoothDevice);
            _DeviceRegistrationMessageHandler.RegisterMethod<
                ConsoleMessage>(
                    Native.MessageTypes.ConsoleMessage,
                    _WebViewMessagingInterface.Send
                );
            _DeviceRegistrationMessageHandler.RegisterMethod<
                ErrorMessage>(
                HVPSAPI.MessageTypes.Error, _WebViewMessagingInterface.Send);
            _DeviceRegistrationMessageHandler.RegisterMethod<
                CoreDumpSummaryMessage>(
                HVPSAPI.MessageTypes.CoreDumpSummary, _WebViewMessagingInterface.Send);
            _DeviceRegistrationMessageHandler.RegisterMethod<
                LastAbortMessage>(
                HVPSAPI.MessageTypes.LastAbort, _WebViewMessagingInterface.Send);
            _DeviceRegistrationMessageHandler.RegisterMethod<
                LastAbortMessage>(
                HVPSAPI.MessageTypes.StateChanged, _WebViewMessagingInterface.Send);
            _DeviceRegistrationMessageHandler.OnMessage += (o, e) => _PingDisconnectDetector.Received();
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
