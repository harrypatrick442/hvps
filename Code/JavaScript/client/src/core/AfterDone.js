import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided }=ParameterErrors;
export default class AfterDone {
    constructor({callback}){
		if(!callback)
			throwNotProvided('callback');
		this._callback = callback;
		this.trigger= this.trigger.bind(this);
	}
	trigger(){
		if(this._triggered)return;
		this._triggered = true;
		setTimeout(()=>{
			this._triggered = false;
			this._callback();
		}, 0);
	}
}