import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
import OrderedItems from './OrderedItems';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
const { throwNotProvided } = ParameterErrors;
export default class TabPanel_Panel {
    constructor({model, createChildView, callbackDispose}) {
		eventEnable(this);
		this.dispose = this.dispose.bind(this);
		this.setActive = this.setActive.bind(this);
		this._callbackDispose = callbackDispose;
		this._element = document.createElement('div');
		this._element.classList.add('panel');
		this._childView = createChildView(model);
		this._element.appendChild(this._childView.element);
	}
	setActive(value){
		if(value)
			this._element.classList.add('active');
		else
			this._element.classList.remove('active');
	}
	dispose(){
		this._element.removeChild(this._childView.element);
		this._childView.dispose&&this._childView.dispose();
		this._callbackDispose(this);
	}
	get element(){
		return this._element;
	}
}