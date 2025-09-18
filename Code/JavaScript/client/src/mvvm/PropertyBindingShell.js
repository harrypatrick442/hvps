export default class PropertyBindingShell{
	constructor(getterMethod, setterMethod){
		this._getMethod = getterMethod;
		this._setMethod =setterMethod;
		this.get = this.get.bind(this);
		this.set = this.set.bind(this);
	}
	get(){
		return this._getMethod();
	}
	set(value){
		this._setMethod(value);
	}
}