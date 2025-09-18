export default class EventNormalizer{
	static addFriendlyXYToTouch_LexicalClosure(callback2){
		return (...args)=>{
			const e = args[0];
			EventNormalizer.addFriendlyXYToTouch(e);
			callback2.apply(null, args);
		};
	}
	static addFriendlyXYToMouse_LexicalClosure(callback){
		return (...args)=>{
			const e = args[0];
			EventNormalizer.addFriendlyXYToMouse(e);
			callback.apply(null, args);
		};
	}
	static addFriendlyXYToTouch(e){
		const touch = e.changedTouches[0];
		if(touch){
			e.x = touch.clientX;
			e.y = touch.clientY;
		}
	}
	static addFriendlyXYToMouse(e){
		try{
		if(e.x===undefined)
			e.x = e.clientX;
		if(e.y===undefined)
			e.y = e.clientY;
		}catch{}
	}
}