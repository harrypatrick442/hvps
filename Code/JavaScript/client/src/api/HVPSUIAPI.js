import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import NativeAPI from './NativeAPI';

import {
	MessageTypes,
	TicketedMessageType,
	GetAvailableBluetoothDevicesRequest,
	GetAvailableBluetoothDevicesResponse,
	ConnectToBluetoothDeviceRequest,
	ConnectToBluetoothDeviceResponse,
	HVPSStartMessage,
	HVPSStopMessage,
	HVPSShutDownMessage,
	HVPSRunSystemChecksOnlyMessage,
	ConsoleMessage,
	DisconnectedMessage
	} from '../messages';
class HVPSUIAPI{
	static shutDown(){
		NativeAPI.send(HVPSShutDownMessage.toJSON());
	}
	static start(){
		NativeAPI.send(HVPSStartMessage.toJSON());
	}
	static stop(){
		NativeAPI.send(HVPSStopMessage.toJSON());
	}
	static runSystemChecksOnly(){
		NativeAPI.send(HVPSRunSystemChecksOnlyMessage.toJSON());
	}
	static getAvailableBluetoothDevices(){
		return NativeAPI.ticketedSend(GetAvailableBluetoothDevicesRequest.toJSON(), 30000)
		.then(res=>GetAvailableBluetoothDevicesResponse.fromJSON(res));
	}
	static connectToBluetoothDevice(address){
		return NativeAPI.ticketedSend(ConnectToBluetoothDeviceRequest.toJSON({address}), 30000)
		.then(res=>ConnectToBluetoothDeviceResponse.fromJSON(res));
	}
	static _handleIncomingMessage({message}){
		console.log(message);
		switch(message[MessageTypes.type]){
			case MessageTypes.bluetoothDeviceDisconnected:
				HVPSUIAPI.dispatchEvent({type:'disconnected'});
				break;
			case MessageTypes.consoleMessage:
				var consoleMessage = ConsoleMessage.fromJSON(message);
				HVPSUIAPI.dispatchEvent({type:'consoleMessage', consoleMessage});
				break;
		}
	}
}
eventEnable(HVPSUIAPI);
NativeAPI.addEventListener('message', HVPSUIAPI._handleIncomingMessage);
export default HVPSUIAPI;