import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
export default class DynamicIcon{
	constructor(src, name, handleMissing){
		eventEnable(this);
		this._src = src;
		this._name = name;
		this._handleMissing=handleMissing;
	}
	get name(){
		return this._name;
	}
	get src(){
		if(isNullOrUndefined(this._src)){
			this._handleMissing(this);
		}
		return this._src;
	}
	set src(value){
		if(this._src===value)return;
		this._src = value;
		this.dispatchEvent({type:'changed', src:value});
	}
}