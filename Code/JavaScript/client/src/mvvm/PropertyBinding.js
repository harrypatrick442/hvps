import enableBindingToOthers from './enableBindingToOthers';
import capitalizeFirstLetter from '../core/capitalizeFirstLetter';
import PropertyHelper from './PropertyHelper';
import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
const {throwNotProvided }= ParameterErrors
export default class  PropertyBinding{
	constructor (me, model, name, valueChanged){
		eventEnable(this);
		this._me = me;
		this._model = model;
		this._name = name;
		this._callbackValueChanged = valueChanged;
		this._disposed=false;
		
		this.set = this.set.bind(this);
		this.get = this.get.bind(this);
		this.getName = this.getName.bind(this);
		this.dispose= this.dispose.bind(this);
		this._dispatchDispose= this._dispatchDispose.bind(this);
		
		const capitalizedName = capitalizeFirstLetter(name);
		this._setterName = PropertyHelper.getSetterName(capitalizedName);
		this._getterName = PropertyHelper.getGetterName(capitalizedName);
		if(!model)throwNotProvided('model');
		if(!model.bindingsHandler)throw new Error('model of type '+model.constructor.name+' has no bindingsHandler. You may need to call enableAcceptBindings on it');
		model.bindingsHandler.addPropertyBinding(this);
		const myPropertyBindings= enableBindingToOthers(me);
		myPropertyBindings.add(this);
	}
	get callbackValueChanged(){
		return this._callbackValueChanged;
	}
	get me(){
		return this._me;
	}
	set(value){
		var setter = this._model[this._setterName];
		if(!setter)
			throw new Error('model of type '+this._model.constructor.name+' does not contain a setter method with name '+this._setterName);
		if(typeof(setter)!=='function')
			throw new Error(this._model.constructor.name+'.'+this._setterName+' is not a function');
		setter(value);
	}
	get(){
		var getter = this._model[this._getterName];
		if(!getter)throw new Error('model of type '+this._model.constructor.name+' does not contain a getter method with name '+this._getterName);
		if(typeof(getter)!=='function')
			throw new Error(this._model.constructor.name+'.'+this._getterName+' is not a function');
		return getter();
	}
	getName(){
		return this._name;
	}
	dispose(){
		if(this._disposed)return;
		this._disposed=true;
		this._dispatchDispose();
	}
	_dispatchDispose(){
		this.dispatchEvent({type:'dispose', propertyBinding:this});
	}
}