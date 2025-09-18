import VirtualSocketMessage from '../messages/VirtualSocketMessage';
export default class VirtualSockets
{
	constructor(){
		this._mapIdToVirtualSocket = new Map();
	}
	add(virtualSocket){
		this._mapIdToVirtualSocket.set(virtualSocket.id, virtualSocket);
	}
	get(id) {
		return this._mapIdToVirtualSocket.get(id);
	}
	handleMessageRaw(message) {
		message = VirtualSocketMessage.fromJSON(message);
		const virtualSocket = this.get(message.id);
		virtualSocket?.handleMessage(message);
	}
	remove(virtualSocket)
	{
		this._mapIdToVirtualSocket.delete(virtualSocket.Id);
	}
	dispose()
	{
		const virtualSockets = Array.from(this._mapIdToVirtualSocket.values);
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