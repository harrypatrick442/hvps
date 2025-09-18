import E from '../ui_core/E';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import isNullOrUndefined from '../core/isNullOrUndefined';
import LoopBreak from '../core/LoopBreak';
export default class Checkbox{
	constructor({model, className, propertyNameValue, 
		propertyNameVisible, text, clickAllOver}){
		this._valueChanged = this._valueChanged.bind(this);
		this._handleChange = this._handleChange.bind(this);
		this._visibleChanged = this._visibleChanged.bind(this);
		this._clickedAllOver = this._clickedAllOver.bind(this);
		this.dispose = this.dispose.bind(this);
		this._loopBreakTicking = new LoopBreak();
		if(isNullOrUndefined(text)){
			this._element = E.checkbox('check-box');
			this._checkboxElement = this._element;
		}
		else{
			this._element = E.div('check-box');
			this._textElement = E.div('text');
			this._textElement.textContent = text;
			this._element.appendChild(this._textElement);
			this._checkboxElement = E.checkbox('checkbox');
			this._element.appendChild(this._checkboxElement);
			if(clickAllOver)
				this._element.addEventListener('click', this._clickedAllOver);
		}
		if(className)
			this._element.classList.add(className);
		this._propertyBindingValue = PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
		if(propertyNameVisible)
			this._propertyBindingVisible = PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
		this._checkboxElement.addEventListener('change', this._handleChange);
	}
	get element(){
		return this._element;
	}
	_visibleChanged(value){
		if(value)
		{
			this._element.classList.add('visible');
		}
		else
		{
			this._element.classList.remove('visible');
		}
	}
	_valueChanged(value){
		this._checkboxElement.checked = value;
		if(value)
		{
			this._element.classList.add('checked');
			this._element.classList.remove('not-checked');
		}
		else
		{
			this._element.classList.add('not-checked');
			this._element.classList.remove('checkbox');
		}
	}
	_clickedAllOver(){
		if(this._loopBreakTicking.triggered)return;
		this._propertyBindingValue.set(!this._propertyBindingValue.get());
	}
	_handleChange(){
		const checked = this._checkboxElement.checked;
		if(this._loopBreakTicking.trigger())return;
		this._propertyBindingValue.set(checked);
	}
	dispose(){
		this.myBindings.dispose();
	}
}