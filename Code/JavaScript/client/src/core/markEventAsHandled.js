import isNullOrUndefined  from './isNullOrUndefined';
if(!window._mapEventNameToHandled)
	window._mapEventNameToHandled = new Map();
export default function markEventAsHandled(a){
	const type = typeof(a)==='string'?a:a?.type;
	window._mapEventNameToHandled.set(type, true);
	setTimeout(()=>{
		window._mapEventNameToHandled.set(type, false);
	}, 0);
}