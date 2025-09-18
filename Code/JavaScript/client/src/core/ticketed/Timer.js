export default class Timer{
	constructor(props){
	   let {delay, callback, nTicks}=props;
		if (nTicks == undefined)
		{
			nTicks = -1;
		}
		if (delay == undefined)
		{
			delay = 1000;
		}
		this._nTicksCount = 0;
		this._delay = delay;
		this._callback = callback;
		this._nTicks = nTicks;
		this._interval = null;
		this._isRunning=false;
		this._tick= this._tick.bind(this);
		this.stop= this.stop.bind(this);
		this.reset= this.reset.bind(this);
		this.start= this.start.bind(this);
		this.setDelay= this.setDelay.bind(this);
		this.isRunning= this.isRunning.bind(this);
		this._setInterval= this._setInterval.bind(this);
		this._cancelInterval= this._cancelInterval.bind(this);
	}
	_tick(){
		if (this._nTicks >= 0)
		{
			this._nTicksCount++;
			if (this._nTicksCount >= this._nTicks)
				this.stop();
		}
		this._callback();
	}
	stop(){
		this._cancelInterval();
	}
	reset(keepRunning)
	{
		this._nTicksCount = 0;
		if(keepRunning)
		{
			if(!this._isRunning)return;
			clearInterval(this._interval);
			this._interval = setInterval(this._tick, this._delay);
		}else{
			this._cancelInterval();
		}
				
	}
	start(){
		if(this._isRunning)return;
		this._setInterval();
	};
	setDelay(value){
		this.reset();
		this._delay = value;
		this._setInterval();
	};
	isRunning(){
		return this._isRunning;
	}
	_setInterval()
	{
		this._isRunning=true;
		this._interval = setInterval(this._tick, this._delay);
	}
	_cancelInterval()
	{
		this._isRunning=false;
		if (!this._interval)return;
		clearInterval(this._interval);
		this._interval=null;
	}
}