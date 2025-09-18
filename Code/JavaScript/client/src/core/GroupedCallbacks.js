import ParameterErrors from '../errors/ParameterErrors';
const {throwNotProvided, throwNotOfType}=ParameterErrors;
export default class GroupedCallbacks {
	constructor(){
		this._callbacksThen=[];
		this._callbacksCatch=[];
		this.then = this.then.bind(this);
		this.catch = this.catch.bind(this);
		this.doThen = this.doThen.bind(this);
		this.doCatch = this.doCatch.bind(this);
	}
	then(callbackThen){
		this._callbacksThen.push(callbackThen);
		return this;
	}
	catch(callbackCatch){
		if(callbackCatch===undefined||callbackCatch===null)throwNotProvided('callbackCatch');
		if(typeof(callbackCatch)!=='function')throwNotOfType('callbackCatch', 'function');
		this._callbacksCatch.push(callbackCatch);
		return this;
	}
	doThen(res){
		const callbacksThen = this._callbacksThen;
		while(callbacksThen.length>0)
		{
			const callbackThen = callbacksThen.splice(0, 1)[0];
			try{
				callbackThen(res);
			}
			catch(err){
				console.error(err);
			}
		}
	}
	doCatch(err){
		const callbacksCatch = this._callbacksCatch;
		while(callbacksCatch.length>0)
		{
			const callbackCatch = callbacksCatch.splice(0, 1)[0];
			try{
				callbackCatch(err);
			}
			catch(err){
				console.error(err);
			}
		}
	}
}