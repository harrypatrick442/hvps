import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided }=ParameterErrors;
export default class DelayedCallback {
    constructor(props){
		const {delay, callback}=props;
		this._delay = delay;
		this._callback = callback;
		this.trigger= this.trigger.bind(this);
	}
	trigger(){
		if(this._triggered)return;
		this._triggered = true;
		setTimeout(()=>{
			this._triggered=false;
			this._callback();
		}, this._delay);
	}
}