import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided }=ParameterErrors;
export default class FrequencyLimitedCallback {
    constructor(props){
		const {frequencyHz, callback}=props;
		this._callback = callback;
		if(frequencyHz===undefined||frequencyHz===null)
			throwNotProvided('frequencyHz');
		this._delay = 1000/frequencyHz;
		this.trigger = this.trigger.bind(this);
		this.cancel = this.cancel.bind(this);
	}
	trigger(callback){
		if(callback){
			this._callback = callback;
		}
		if(this._pausing){
			this._triggered = true;
			return;
		}
		else 
			this._callback();
		this._pausing=true;
		var doPause = null;
		doPause = ()=>{
			setTimeout(()=>{
				if(this._triggered){
					this._triggered=false;
					doPause();
					this._callback();
				}
				else 
					this._pausing=false;
			}, this._delay);
		};
		doPause();
	}
	cancel(){
		this._triggered = false;
	}
}