import eventEnable from '../core/eventEnable';
import VirtualSocketMessage from '../messages/VirtualSocketMessage';
import isNullOrUndefined from '../core/isNullOrUndefined';
import MessageTypes from '../messages/MessageTypes';
import ObjectDisposedError from '../errors/ObjectDisposedError';
export default class VirtualSocket
{
	constructor({id, send, virtualSockets}){
		eventEnable(this);
		if(!id) throw new Error('id');
		if(!send) throw new Error('send');
		if(!virtualSockets) throw new Error('virtualSockets');
		this.handleMessage= this.handleMessage.bind(this);
		this.send= this.send.bind(this);
		this.dispose= this.dispose.bind(this);
		this._disposed = false;
		this._id = id;
		this._send = send;
		this._virtualSockets = virtualSockets;
		this._virtualSockets.add(this);
	}
	get id(){
		return this._id;
	}
	handleMessage({internalType, payload, dispose}){
		if(this._disposed)return;
		if(dispose){
			this.dispose();
		}
		payload = isNullOrUndefined(payload)?null:JSON.parse(payload);
		try{
		this.dispatchEvent({type:'message', internalType:internalType, payload});
		}
		catch(err){console.error(err);}
		if(internalType===MessageTypes.virtualSocketDisposing)
			this.dispose();
	}
	send(type, message) {
		if(this._disposed)throw new ObjectDisposedError();
		if(!type) throw new Error('type');
		this._send(VirtualSocketMessage.toJSON({id:this._id, payload:JSON.stringify(message), internalType:type}));
	}
	dispose() {
		if(this._disposed)return;
		this._disposed = true;
		this._virtualSockets.remove(this);
		try{
			this.dispatchEvent('dispose', {});
		}
		catch(err){
			console.error(err);
		}
		try{
			this._send(VirtualSocketMessage.toJSON({id:this._id, payload:null,
				internalType:MessageTypes.virtualSocketDisposing}));
		}catch{}
	}
}