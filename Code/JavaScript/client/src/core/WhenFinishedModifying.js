import ParameterErrors from '../errors/ParameterErrors';
import isNullOrUndefined from '../core/isNullOrUndefined';
const {throwNotProvided }=ParameterErrors;
export default class WhenFinishedModifying {
    constructor({delay, callback}){
		this._delay = delay;
		if(isNullOrUndefined(delay))
			throwNotProvided('delay');
		if(!callback)
			throwNotProvided('callback');
		this._callback = callback;
		this._currentTimeout= null;
		this.trigger= this.trigger.bind(this);
		this.now = this.now.bind(this);
	}
	trigger(){
		if(this._currentTimeout)
			clearTimeout(this._currentTimeout);
		this._currentTimeout = setTimeout(()=>{
			this._callback();
		}, this._delay);
	}
	now(){
		if(this._currentTimeout)
			clearTimeout(this._currentTimeout);
		this._callback();
	}
}