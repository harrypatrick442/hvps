import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import E from '../../ui_core/E';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import './ValueAndMaxField.scss';
import RoundingHelper from '../../core/RoundingHelper';
import ValueBoundType from '../../enums/ValueBoundType';
export default class ValueAndMaxField{
	constructor({
			text,
			units,
			className, 
			propertyNameValue, 
			propertyNameMax,
			propertyNameValueBoundType,
			model,
			nSigFigures = 3
	}){
		this._nSigFigures = nSigFigures;
		this.dispose = this.dispose.bind(this);
		this._valueChanged = this._valueChanged.bind(this);
		this._maxChanged = this._maxChanged.bind(this);
		this._valueBoundTypeChanged = this._valueBoundTypeChanged.bind(this);
		this._updateValue = this._updateValue.bind(this);
		this._updateMax = this._updateMax.bind(this);
		this._round = this._round.bind(this);
		this._update = this._update.bind(this);
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
		const valueLabelElement = E.div('value-label');
		valueLabelElement.textContent = 'Live:';
		secondRowElement.appendChild(valueLabelElement);
		secondRowElement.appendChild(this._valueElement);
		const hasMax = !isNullOrUndefined(propertyNameMax);
		if(hasMax){
			const maxLabelElement = E.div('max-label');
			maxLabelElement.textContent = 'Max:';
			secondRowElement.appendChild(maxLabelElement);
			this._maxElement = E.div('max');
			secondRowElement.appendChild(this._maxElement);
			PropertyBindingFactory.standard(this, model, propertyNameMax, this._maxChanged);
		}
		PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
		if(propertyNameValueBoundType){
			PropertyBindingFactory.standard(this, model, propertyNameValueBoundType, this._valueBoundTypeChanged);
		}
	}
	get element(){
		return this._element;
	}
	dispose(){
		this.myBindings.dispose();
	}
	_valueChanged(value){
		this._value = value;
		this._updateValue();
	}
	_maxChanged(value){
		this._max = value;
		this._updateMax();
	}
	_round(value){
		if(isNullOrUndefined(this._nSigFigures))
			return value;
		return RoundingHelper.toSignificantFigures(value, this._nSigFigures);
	}
	_valueBoundTypeChanged(value){
		this._valueBoundType = value;
		this._updateValue();
	}
	_updateValue(){
		this._update(this._value, this._valueElement, true);
	}
	_updateMax(){
		this._update(this._max, this._maxElement);
	}
	_update(value, element, useValueBound){
		if(isNullOrUndefined(value)){
			element.innerHTML = 'Unknown';
			return;
		}
		var str = '';
		if(useValueBound){
			if(this._valueBoundType===ValueBoundType.MinimumKnown){
				str+='<= ';
			}
		}
		str+=String(this._round(value));
		element.innerHTML = str;
	}
}