export default class VirtualSocketIdSource
{
	static _nextId = 1;
	static get nextId(){
		const id = this._nextId;
		this._nextId++;
		return id;
	}
}