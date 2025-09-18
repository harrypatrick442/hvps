import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided}=ParameterErrors;
export default class Once{
	constructor(callback){//multiple triggers synchronously results in one call to the callback.
		this._callback = callback;
		if(callback===undefined||callback===null)
			throwNotProvided('callback');
		this._set=false;
		this.trigger = this.trigger.bind(this);
		this._unsetIt=this._unsetIt.bind(this);
		this._setIt = this._setIt.bind(this);
	}
	trigger(a){
		if(!this._set){
			this._setIt(arguments);
		}
	};
	_unsetIt(){
		this._set=false;
	}
	_setIt(args){
		setTimeout(this._unsetIt, 0);
		this._set=true;
		this._callback.apply(null, args);
	}
}