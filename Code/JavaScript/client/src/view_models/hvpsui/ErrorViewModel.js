import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import exposeBinding  from '../../mvvm/exposeBinding';
import exposeMethod  from '../../mvvm/exposeMethod';
import UrlParameters from '../../urls/UrlParameters';
import eventEnable  from '../../core/eventEnable';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import isNullOrUndefinedOrEmptyString  from '../../core/isNullOrUndefinedOrEmptyString';
export default class ErrorViewModel{
	constructor(errorMessage){
		eventEnable(this);
		const disposes = [];
		this._disposes = disposes;
		this.dispose = this.dispose.bind(this);
		this._errorType = errorMessage.errorType;
		//exposeBinding(this, 'devices', ()=>this.devices);
		
	}
	get errorType(){
		return this._errorType;
	}
	dispose(){
		this._disposes.forEach(d=>d());
	}
}