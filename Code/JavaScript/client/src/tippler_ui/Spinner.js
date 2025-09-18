import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import './Spinner.css';
export default class Spinner {
	constructor({model, propertyNameVisible}){
		this.dispose = this.dispose.bind(this);
		this._visibleChanged = this._visibleChanged.bind(this);
		this._element = document.createElement('div');
		this._element.classList.add("spinner");
		for(var i=0; i<3; i++){
			this._element.appendChild(document.createElement('div'));
		}
		if(propertyNameVisible)
			PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
	}
	get element(){
		return this._element;
	}
	_visibleChanged(value){
		if(value)
			this._element.classList.add('visible');
		else
			this._element.classList.remove('visible');
	}
	dispose(){
		this.myBindings?.dispose();
	}
}
