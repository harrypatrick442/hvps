import VirtualSocket2Message from '../messages/VirtualSocket2Message';
export default class VirtualSocket2s
{
	constructor(){
		this._mapSecretToVirtualSocket = new Map();
	}
	add(virtualSocket){
		this._mapSecretToVirtualSocket.set(virtualSocket.secret, virtualSocket);
	}
	get(secret) {
		return this._mapSecretToVirtualSocket.get(secret);
	}
	handleMessageRaw(message) {
		message = VirtualSocket2Message.fromJSON(message);
		const virtualSocket = this.get(message.secret);
		console.log('Virtual socket message was ');
		console.log(message);
		console.log(virtualSocket);
		virtualSocket?.handleMessage(message);
	}
	remove(virtualSocket)
	{
		this._mapSecretToVirtualSocket.delete(virtualSocket.secret);
	}
	dispose()
	{
		const virtualSockets = Array.from(this._mapSecretToVirtualSocket.values);
		for(var virtualSocket of virtualSockets)
		{
			try
			{
				virtualSocket.dispose();
			}
			catch (err)
			{
				console.error(err);
			}
		}
	}
}