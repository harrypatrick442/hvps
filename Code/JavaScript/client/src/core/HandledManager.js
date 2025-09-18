import isNullOrUndefined  from './isNullOrUndefined';
export default (new (function HandledManager(){
	const mapEventNameToHandled = new Map();	
	const scheduleTimeout = (type)=>{
		setTimeout(()=>{
			window._mapEventNameToHandled.set(type, false);
		}, 0);
	};
	this.handled = (a)=>{
		const type = typeof(a)==='string'?a:a.type;
		const handled = window._mapEventNameToHandled.get(type);
		if(handled)return true;
		window._mapEventNameToHandled.set(type, true);
		scheduleTimeout(type);
		return false;
	};
	this.handledKeyCode = (keyCode)=>this.handled(`keyCode_${keyCode}`);
})());