import HVPSState from '../../enums/HVPSState';
import E from '../../ui_core/E';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import './State.scss';
export default class State{
	constructor({propertyName, model}){
		this._element = E.div('state');
		this._stateChanged = this._stateChanged.bind(this);
		this._currentState = HVPSState.Unknown;
		PropertyBindingFactory.standard(this, model, propertyName, this._stateChanged);
	}
	get element(){
		return this._element;
	}
	_stateChanged(hvpsState){
		if(!isNullOrUndefined(this._currentState)){
			this._element.classList.remove(HVPSState.getClassName(this._currentState));
		}
		this._currentState = hvpsState;
		this._element.classList.add(HVPSState.getClassName(hvpsState));
		const text = HVPSState.getDescription(hvpsState);
		this._element.textContent = isNullOrUndefined(text)?'':`Device State: ${text}`;
	}
}