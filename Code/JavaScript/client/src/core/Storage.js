import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import NativeAPI from '../api/NativeAPI';
const ALLOW_ALL_NAME_IN_STORAGE = 'allowAllCookies';
const {throwNotProvided}=ParameterErrors;
const _forIfAllows= new Map();
class Storage{
	static initialize(){
		return Storage._getAllowedAll()
		.then((value)=>{
			Storage._allowed=value;
			Storage._dispatchAllowedChanged(value);
			return value;
		})
		.catch(err=>{
			console.error(err);
			Storage._allowed=null;
			return null;
		})
	}
	static _getAllowedAll(){
		if(NativeAPI.isNative){
			return NativeAPI.storageGetString(ALLOW_ALL_NAME_IN_STORAGE) 
			.then(value=>{
				return isNullOrUndefined(value)?null:JSON.parse(value);
			}).catch(()=>{
				return null;
			});
		}
		try{
		const stringValue = window.localStorage?.getItem(ALLOW_ALL_NAME_IN_STORAGE);
		const value = isNullOrUndefined(stringValue)?null:JSON.parse(stringValue);
			return Promise.resolve(value);
		}
		catch(err){
			console.error(err);
			return Promise.resolve(null);
		}
	}
	static setItem(key, value){
		Storage.setString(key, isNullOrUndefined(value)?null:JSON.stringify(value));
	}
	static getItem(key){
		return Storage.getString(key).then((stringValue)=>{
			if(!isNullOrUndefined(stringValue)){
				try{ return JSON.parse(stringValue);}
				catch{}
			}
			return null;
		});
	}
	static setString(key, value){
		try{
		const isNull = isNullOrUndefined(value);
		if(!Storage._allowed)
		{
			_forIfAllows.set(key, value);
			return;
		}
		if(NativeAPI.isNative){
			NativeAPI.storageSetString(key, isNull
				?null:value);
			return;
		}
		if(isNull)
			window.localStorage?.removeItem(key);
		else
			window.localStorage?.setItem(key, value);
		}
		catch(err){
			console.error(err);
		}
	}
	static getString(key){
		let value;
		if(!Storage._allowed)
		{
			if(_forIfAllows.has(key))
				return Promise.resolve(_forIfAllows.get(key));
		}
		if(NativeAPI?.isNative){
			return NativeAPI.storageGetString(key);
		}
		return Promise.resolve(window.localStorage?.getItem(key));
	}
	static allowAll(){
		Storage._allowed = true;
		this.setItem(ALLOW_ALL_NAME_IN_STORAGE, true);
		_forIfAllows.forEach((value, key)=>{
			this.setItem(key, value);
		});
		Storage._dispatchAllowedChanged(true);
	}
	static rejectAll(){
		_forIfAllows.clear();
		if(NativeAPI.isNative){
			NativeAPI.storageDeleteAll();
			NativeAPI.storageSetString(
			ALLOW_ALL_NAME_IN_STORAGE, JSON.stringify(false));
		}
		else{
			window.localStorage?.clear();
			window.localStorage?.setItem(
				ALLOW_ALL_NAME_IN_STORAGE, JSON.stringify(false));
		}
		Storage._dispatchAllowedChanged(false);
	}
	static _dispatchAllowedChanged(value){
		Storage.dispatchEvent({type:'allowedChanged', value});
	}
}
eventEnable(Storage);
export default Storage;