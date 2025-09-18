export default class LoopBreak{
	constructor(props){
		this.trigger= this.trigger.bind(this);
		this.untrigger= this.untrigger.bind(this);
		this._triggered=false;
	}
	get triggered(){
		return this._triggered;
	}
	trigger(){
		if(this._triggered)return true;
		this._triggered=true;
		setTimeout(()=>{this._triggered=false;}, 0);
		return false;
	};
	untrigger(){
		this._triggered = false;
	}
};