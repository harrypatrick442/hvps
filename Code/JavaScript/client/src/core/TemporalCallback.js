import Timer from './Timer';
export default class TemporalCallback{
	constructor({callback, maxNTriggers, maxTotalDelay, delay}){
		this._callback = callback;
		this._maxNTriggers = maxNTriggers;
		this._maxTotalDelay = maxTotalDelay;
		this._delay = delay;
		
		this.trigger = this.trigger.bind(this);
		this._tick = this._tick.bind(this);
		
		this._timerDelay = new Timer({callback:this._tick, delay:delay, nTicks:1});
		if(maxTotalDelay!==undefined&&maxTotalDelay!==null)
			this._timerMaxDelay = new Timer({callback:this._tick, delay:maxTotalDelay, nTicks:1});
		this._nTriggers=0;
	}
	trigger(){
		this._nTriggers++;
		if(!this._timerDelay.isRunning()){
			this._timerDelay.start();
			this._timerMaxDelay&&this._timerMaxDelay.start();
			return;
		}
		if(this._maxNTriggers&&this._nTriggers>this._maxNTriggers){
			this._tick();
			return;
		}
		this._timerDelay.reset(true);
	}
	_tick(){
		this._nTriggers++;
		this._timerDelay.reset();
		this._timerMaxDelay&&this._timerMaxDelay.reset();
		this._nTriggers=0;
		this._callback();
	}
}