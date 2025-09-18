import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import E from '../../ui_core/E';
import './ValueAndMaxField.scss';
export default class ValueAndMaxField{
	constructor({
			text,
			units,
			className, 
			propertyNameValue, 
			propertyNameMax, 
			model
	}){
		this.dispose = this.dispose.bind(this);
		this._valueChanged = this._valueChanged.bind(this);
		this._maxChanged = this._maxChanged.bind(this);
		const element = E.div('value-and-max-field');
		const innerElement = E.div('inner');
		element.appendChild(innerElement);
		this._element = element;
		const labelElement = E.div('label');
		labelElement.textContent = text;
		const unitsElement = E.div('units');
		unitsElement.textContent = `(${units})`;
		labelElement.appendChild(unitsElement);
		innerElement.appendChild(labelElement);
		const secondRowElement = E.div('second-row');
		innerElement.appendChild(secondRowElement);
		this._valueElement = E.div('value');
		secondRowElement.appendChild(this._valueElement);
		const maxLabelElement = E.div('max-label');
		maxLabelElement.textContent = 'Max:';
		secondRowElement.appendChild(maxLabelElement);
		this._maxElement = E.div('max');
		secondRowElement.appendChild(this._maxElement);
		PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
		PropertyBindingFactory.standard(this, model, propertyNameMax, this._maxChanged);
	}
	get element(){
		return this._element;
	}
	dispose(){
		this.myBindings.dispose();
	}
	_valueChanged(value){
		this._valueElement.innerHTML = String(value);
	}
	_maxChanged(value){
		this._maxElement.innerHTML = String(value);
	}
}