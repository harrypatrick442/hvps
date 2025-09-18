import PropertyBindingFactory from '../mvvm/PropertyBindingFactory'; 
import E from '../ui_core/E';
import './Blocker.css';
export default class Blocker {
	constructor({model, propertyNameBlocking}){
		this._blockingChanged = this._blockingChanged.bind(this);
		this._element = E.div('blocker');
		PropertyBindingFactory.standard(this, model, propertyNameBlocking, this._blockingChanged);
	}
	get element(){
		return this._element;
	}
	_blockingChanged(value){
		if(value)
			this._element.classList.add('blocking');
		else
			this._element.classList.remove('blocking');
	}
	dispose(){
		this.myBindings.dispose();
	}
}
