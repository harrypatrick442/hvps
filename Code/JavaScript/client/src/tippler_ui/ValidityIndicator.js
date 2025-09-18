import PropertyHelper from '../mvvm/PropertyHelper';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
const CLASS_NAME_INVALID_BORDER='invalid-border';
const CLASS_NAME_INVALID_BACKGROUND='invalid-background';
export default class ValidityIndicator{
	constructor(me, model, a, b, c){
		if(a===undefined||a===null)throw new Error('Arguments not valid');
		if(typeof(a)=='string'){
			this._propertyNameValue = a;
			this._element = b;
			this._classNameInvalid = c;
		}
		else{
			this._propertyNameValue = a.propertyNameValue;
			this._element = a.element;
			this._classNameInvalid = b;
		}
		if(!this._classNameInvalid)
		{
			var validatedElementTagName = this._element.tagName;
			switch(validatedElementTagName){
				case 'select':
				case 'input':
				default:
					this._classNameInvalid=CLASS_NAME_INVALID_BORDER;
			}
		}
		this._changed = this._changed.bind(this);
		const propertyNameValid = this._propertyNameValue+'Valid';
		const validGetterName = PropertyHelper.getGetterName(propertyNameValid);
		if(!model[validGetterName])
			throw new Error('Model of type '+model.constructor.name+' does not contain a valid property '+validGetterName);
		const propertyBinding = PropertyBindingFactory.standard(me, model, propertyNameValid, this._changed);
		this._changed(propertyBinding.get());
	};
	_changed(value){
		if(value)this._element.classList.remove(this._classNameInvalid);
		else this._element.classList.add(this._classNameInvalid);
	}
	static newBorder(me, model, a, b){
		return arguments.length>3
		?new ValidityIndicator(me, model, a, b, CLASS_NAME_INVALID_BORDER)
		:new ValidityIndicator(me, model, a, CLASS_NAME_INVALID_BORDER);
	}
	static newBackground(me, model, a, b){
		return arguments.length>3
		?new ValidityIndicator(me, model, a, b, CLASS_NAME_INVALID_BACKGROUND)
		:new ValidityIndicator(me, model, a, CLASS_NAME_INVALID_BACKGROUND);
	}
	/*function Indicator(){
		var element = E.DIV();
		element.classList.add('validity-indicator');
		element.innerHTML = invalidText;
		this['setVisible']=function(value){
			if(value)
				element.classList.add('visible');
			else
				element.classList.remove('visible');
		};	
	}*/
}