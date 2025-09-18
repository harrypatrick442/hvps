import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import E from '../ui_core/E';
import isNullOrUndefined from '../core/isNullOrUndefined';
import './VerticalMenu.css';
export default class VerticalMenu{
	constructor({
			model,
			items,
			propertyNameActiveItemValue
		})
	{
		this._handleClicked = this._handleClicked.bind(this);
		this._activeItemValueChanged = this._activeItemValueChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._element = E.div('vertical-menu');
		this._mapValueToElement = new Map();
		items.forEach(i=>{
			const {text, value}=i;
			const element = E.button('button');
			element.textContent = text;
			element.addEventListener('click', ()=>this._handleClicked(value));
			this._element.appendChild(element);
			this._mapValueToElement.set(value, element);
		});
		this._propertyBindingActiveItemValue = PropertyBindingFactory.standard(
			this, model, propertyNameActiveItemValue, this._activeItemValueChanged);
	}
	get element(){
		return this._element;
	}
	_handleClicked(value){
		this._propertyBindingActiveItemValue.set(value);
	}
	_activeItemValueChanged(value){
		const element = this._mapValueToElement.get(value);
		if(this._currentActiveElement)
			this._currentActiveElement.classList.remove('active');
		if(isNullOrUndefined(element))return;
		this._element.removeChild(element);
		this._element.appendChild(element);
		this._currentActiveElement = element;
		element.classList.add('active');
	}
	dispose(){
		this.myBindings?.dispose();
	}
}