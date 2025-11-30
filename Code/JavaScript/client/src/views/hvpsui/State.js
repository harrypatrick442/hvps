import SystemState from '../../enums/SystemState';
import E from '../../ui_core/E';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import './State.scss';
export default class State{
	constructor({propertyName, model}){
		this._element = E.div('state');
		this._stateChanged = this._stateChanged.bind(this);
		this._currentState = SystemState.Unknown;
		PropertyBindingFactory.standard(this, model, propertyName, this._stateChanged);
	}
	get element(){
		return this._element;
	}
	_stateChanged(systemState){
		if(!isNullOrUndefined(this._currentState)){
			this._element.classList.remove(SystemState.getClassName(this._currentState));
		}
		this._currentState = systemState;
		this._element.classList.add(SystemState.getClassName(systemState));
		const text = SystemState.getDescription(systemState);
		this._element.textContent = isNullOrUndefined(text)?'':`Device State: ${text}`;
	}
}