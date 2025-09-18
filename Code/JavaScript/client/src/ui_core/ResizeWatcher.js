
import eventEnable from '../core/eventEnable';
import Timer from '../core/Timer';
import TemporalCallback from '../core/TemporalCallback';
export default class ResizeWatcher{
	constructor({element, onResized, onFirstResize, staggered}){
		eventEnable(this);
		this._element = element;
		this._onResized= onResized;
		this._onFirstResize= onFirstResize;
		
		this.manual= this.manual.bind(this);
		this.dispose= this.dispose.bind(this);
		this._resizedCallbackNotInitial= this._resizedCallbackNotInitial.bind(this);
		this._logSize= this._logSize.bind(this);
		this._scheduleResize= this._scheduleResize.bind(this);
		this._finishedResizing= this._finishedResizing.bind(this);
		this._resized= this._resized.bind(this);
		this._doStartIfRequired= this._doStartIfRequired.bind(this);
		this.__resized= this.__resized.bind(this);
		this._dispatchResized= this._dispatchResized.bind(this);
		
		this._temporalCallback = staggered?new TemporalCallback({callback:this.__resized, delay:500, maxTotalDelay:800}):undefined;
		this._timerFinishedResizing = new Timer({nTicks:1, delay:900, callback:this._finishedResizing});
		this._startedResizing = false;
		this._loggedSize=null;
		this._resizeObserver= null;
		this._hasResizeObserver=window.ResizeObserver?true:false;
		this._resizedCallback=staggered?this._scheduleResize: this._resized;
		if(this._hasResizeObserver)
		{
			this._resizeObserver = new (window.ResizeObserver)(this._resizedCallbackNotInitial);
			this._resizeObserver.observe(this._element);
		}
		else
			window.addEventListener("resize", this._resizedCallback);

		setTimeout(()=>{
			this._resizedCallbackForNotInitial=this._resizedCallback;
			}
		,0);
		
	}
	manual(){
		this._resized();
	}
	dispose(){
		if(!this._hasResizeObserver)
			window.removeEventListener("resize", this._resizedCallback);
		else
			this._resizeObserver['disconnect']();
	}
	_resizedCallbackNotInitial(){
		try{
		this._resizedCallbackForNotInitial&&this._resizedCallbackForNotInitial();
		}
		catch(err){
			console.error(err);
		}
	}
	_logSize(){this._loggedSize= this._element['getBoundingClientRect']();}
	_scheduleResize(){
		this._temporalCallback.trigger();
		this._doStartIfRequired();
	}
	_finishedResizing(){
		this._startedResizing=false;
	}
	_resized(params){
		this._doStartIfRequired();
		this.__resized();
	}
	_doStartIfRequired(){
		if(!this._startedResizing){
			this._onFirstResize&&this._onFirstResize();
			this._timerFinishedResizing.start();
			this._startedResizing = true;
		}else{
			this._timerFinishedResizing.reset(true);
		}
	}
	__resized(params){
		if(!this._loggedSize){this._logSize(); return;}
		var previousLoggedSize= this._loggedSize;
		this._logSize();
		if(previousLoggedSize['height']==this._loggedSize['height']&&previousLoggedSize['width']==this._loggedSize['width'])return;
		this._dispatchResized();
	}
	_dispatchResized(){
		this.dispatchEvent({type:'resized'});
		this._onResized&&this._onResized();
	}
}