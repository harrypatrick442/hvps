import Urls from '../urls/Urls';
import eventEnable from '../core/eventEnable';
import GroupedCallbacks from '../core/GroupedCallbacks';
import TicketedSender from '../core/TicketedSender';
import isNullOrUndefined from '../core/isNullOrUndefined';
import VirtualSocketMessage from '../messages/VirtualSocketMessage';
import NativeVirtualSockets from '../native/NativeVirtualSockets';

import {
	MessageTypes,
	TicketedMessageType,
	NativePlatformMessage,
	NativeReadyMessage,
	NativePermissionsUpdateMessage,
	NativeStorageGetStringRequest,
	NativeStorageGetStringResponse,
	NativeStorageSetStringRequest,
	NativeStorageSetStringResponse,
	NativeStorageDeleteAllRequest,
	NativeStorageDeleteAllResponse,
	NativePickFileRequest,
	NativePickFileResponse,
	NativeShowSaveFilePickerRequest,
	NativeShowSaveFilePickerResponse
	} from '../messages';
const TIMEOUT_MILLISECONDS = 3000;
class NativeAPI{
	static get isNative(){
        const value = (window.NativeToJavaScriptInterface
		&&window.NativeToJavaScriptInterface.send)||(
			window.webkit&&window.webkit.messageHandlers&&
			window.webkit.messageHandlers.NativeToJavaScriptInterface
				.postMessage)?true:false
		||(window.chrome?.webview?.postMessage)?true:false;
		return value;
	}
	static get platform(){
		return NativeAPI._platform;
	}
	static reloadPage(){
		NativeAPI.send({
			[MessageTypes.type]:MessageTypes.nativeReloadPage
		});
	}
	static requestPermissions(){
		NativeAPI.send({
			[MessageTypes.type]:MessageTypes.nativeRequestPermissions
		});
	}
	static pickFile(){
		return NativeAPI.ticketedSend(NativePickFileRequest.toJSON(), 2147483647)
		.then((res)=>{
			return NativePickFileResponse.fromJSON(res);
		})
		.catch((err)=>console.error(err));
	}
	static showSaveFilePicker({fileInfo}){
		return NativeAPI.ticketedSend(NativeShowSaveFilePickerRequest.toJSON({fileInfo}), 2147483647)
		.then((res)=>{
			return NativeShowSaveFilePickerResponse.fromJSON(res);
		})
		.catch((err)=>console.error(err));
	}
	static storageGetString(key){
		return NativeAPI.ticketedSend(NativeStorageGetStringRequest.toJSON({key}))
		.then(NativeStorageGetStringResponse.fromJSON)
		.then(obj=>isNullOrUndefined(obj)?null:obj.value);
	}
	static storageSetString(key, value){
		return NativeAPI.ticketedSend(NativeStorageSetStringRequest.toJSON({key, value}))
		.then(NativeStorageSetStringResponse.fromJSON);
	}
	static storageDeleteAll(){
		return NativeAPI.ticketedSend(NativeStorageDeleteAllRequest.toJSON({}))
		.then(NativeStorageDeleteAllResponse.fromJSON);
	}
	static _ready(){
		NativeAPI.send(NativeReadyMessage.toJSON({}));
	}
	static ticketedSend(msg, timeoutMilliseconds){
		
		console.log(msg);
		return TicketedSender.send({socket:NativeAPI, msg, timeoutMs:isNullOrUndefined(timeoutMilliseconds)?TIMEOUT_MILLISECONDS:timeoutMilliseconds});
	}
	static send(message){
		NativeAPI.sendRawMessageToDotNet(JSON.stringify(message));
	}
    static sendRawMessageToDotNet(jsonString) {
		if(window.NativeToJavaScriptInterface){
			window.NativeToJavaScriptInterface.send(jsonString);
			return;
		}
		if(window.webkit?.messageHandlers?.NativeToJavaScriptInterface)
		{
			window.webkit.messageHandlers.NativeToJavaScriptInterface
				.postMessage(jsonString);
		}
		if(window.chrome?.webview?.postMessage)
		{
			window.chrome.webview.postMessage(jsonString);
		}
    }
	static _incomingMessage(message){
		NativeAPI.dispatchEvent({type:'message', message});
		switch(message[MessageTypes.type]){
			case MessageTypes.nativePlatform:
				const nativePlatformMessage = NativePlatformMessage.fromJSON(message);
				NativeAPI._platform = nativePlatformMessage.platform;
				break;
			case MessageTypes.nativePermissionsUpdate:
				const {hasAllRequired} = NativePermissionsUpdateMessage.fromJSON(message);
				NativeAPI.dispatchEvent({type:'gotNativePermissionsUpdate', hasAllRequired});
				break;
		}
	}
}
console.log('Running NativeAPI');
eventEnable(NativeAPI);
window['_incomingNative']=NativeAPI._incomingMessage;
window.chrome?.webview?.addEventListener('message', event => {
    NativeAPI._incomingMessage(event.data);
});
console.log('isNative');
console.log(NativeAPI.isNative);
if(NativeAPI.isNative)
	NativeAPI._ready();
export default NativeAPI;