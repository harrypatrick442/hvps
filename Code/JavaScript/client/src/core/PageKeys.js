import isNullOrUndefined from './isNullOrUndefined';
let instance = null;
export default class PageKeys{
	static get instance(){
		if(isNullOrUndefined(instance)){
			instance = new PageKeys();
		}
		return instance;
	}
	constructor(){
		this.registerKeyDown = this.registerKeyDown.bind(this);
		this.registerKeyUp = this.registerKeyUp.bind(this);
		this.registerSpecificKeyDown = this.registerSpecificKeyDown.bind(this);
		this.registerSpecificKeyUp = this.registerSpecificKeyUp.bind(this);
		this._handleKeyDown = this._handleKeyDown.bind(this);
		this._handleKeyUp = this._handleKeyUp.bind(this);
		this._keyDownCallbacks = new Set()
		this._keyUpCallbacks = new Set();
		document.documentElement.addEventListener('keydown', this._handleKeyDown);
		document.documentElement.addEventListener('keyup', this._handleKeyUp);
	}
	registerKeyDown(callback){
		this._keyDownCallbacks.add(callback);
		return ()=>this._keyDownCallbacks.delete(callback);
	}
	registerKeyUp(callback){
		this._keyUpCallbacks.add(callback);
		return ()=>this._keyUpCallbacks.delete(callback);
	}
	registerSpecificKeyDown(keyCode_s, callback){
		if(!Array.isArray(keyCode_s))
			keyCode_s = [keyCode_s];
		return this.registerKeyDown((e)=>{
			const {keyCode} = e;
			if(keyCode_s.indexOf(keyCode)>=0){
				callback(e);
			}
		});
	}
	registerSpecificKeyUp(keyCode_s, callback){
		if(!Array.isArray(keyCode_s))
			keyCode_s = [keyCode_s];
		return this.registerKeyUp((e)=>{
			const {keyCode} = e;
			if(keyCode_s.indexOf(keyCode)>=0){
				callback(e);
			}
		});
	}
	_handleKeyDown(e){
		this._keyDownCallbacks.forEach(c=>{
			try{
				c(e);
			}
			catch(err){
				console.error(err);
			}
		});
	}
	_handleKeyUp(e){
		this._keyUpCallbacks.forEach(c=>{
			try{
				c(e);
			}
			catch(err){
				console.error(err);
			}
		});
	}
}