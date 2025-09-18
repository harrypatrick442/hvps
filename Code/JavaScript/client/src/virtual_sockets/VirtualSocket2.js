import eventEnable from '../core/eventEnable';
import VirtualSocket2Message from '../messages/VirtualSocket2Message';
import isNullOrUndefined from '../core/isNullOrUndefined';
import MessageTypes from '../messages/MessageTypes';
import ObjectDisposedError from '../errors/ObjectDisposedError';
export default class VirtualSocket2
{
	constructor({newVirtualSocket2, send, virtualSocket2s}){
		eventEnable(this);
		const{secret, theirNodeId, endpointId}=newVirtualSocket2;
		if(isNullOrUndefined(secret)) throw new Error('secret');
		if(isNullOrUndefined(theirNodeId)) throw new Error('theirNodeId');
		if(isNullOrUndefined(endpointId)) throw new Error('endpointId');
		if(isNullOrUndefined(send)) throw new Error('send');
		if(isNullOrUndefined(virtualSocket2s)) throw new Error('virtualSocket2s');
		this._secret = secret;
		this._theirNodeId = theirNodeId;
		this._endpointId = endpointId;
		this._send = send;
		this._virtualSocket2s=virtualSocket2s;
		this.handleMessage= this.handleMessage.bind(this);
		this.send= this.send.bind(this);
		this.dispose= this.dispose.bind(this);
		this._disposed = false;
		virtualSocket2s.add(this);
	}
	get secret(){
		return this._secret;
	}
	handleMessage({payload}){
		if(this._disposed)return;
		if(isNullOrUndefined(payload))return;
		payload = JSON.parse(payload);
		try{
			this.dispatchEvent({type:'message', payload});
		}
		catch(err){console.error(err);}
		if(payload[MessageTypes.type]===MessageTypes.virtualSocketDisposing)
			this.dispose();
	}
	send(message) {
		if(this._disposed)throw new ObjectDisposedError();
		this._send(VirtualSocket2Message.toJSON({secret:this._secret, theirNodeId:this._theirNodeId, 
			endpointId:this._endpointId, payload:JSON.stringify(message)}));
	}
	dispose() {
		if(this._disposed)return;
		this._disposed = true;
		this._virtualSocket2s.remove(this);
		try{
			this.dispatchEvent('dispose', {});
		}
		catch(err){
			console.error(err);
		}
		try{
			this.send({[MessageTypes.type]:MessageTypes.virtualSocketDisposing});
		}catch{}
	}
}