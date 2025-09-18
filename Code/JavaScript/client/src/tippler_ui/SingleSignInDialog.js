import i from '../ui_core/i';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import TextBox from './TextBox';
import E from '../ui_core/E';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import './SingleSignInDialog.css';
export default class SingleSignInDialog{
	constructor({model, methodNameSignIn, propertyNamePassword, propertyNameVisible}){
		const disposes = [];
		this._disposes = disposes;
		this.dispose = this.dispose.bind(this);
		this._visibleChanged = this._visibleChanged.bind(this);
		this._element = E.div('single-sign-in-dialog');
		this._password = new TextBox({
			className:'password', placeholder:'password', isPassword:true, 
			propertyName:propertyNamePassword, model
		});
		this._buttonSignIn = _createImageHoverButton(i('Enter'), i('EnterWhite'),
			'button-sign-in', model[methodNameSignIn], disposes, true);
		this._element.appendChild(this._password.element);
		this._element.appendChild(this._buttonSignIn);
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
		this._disposes.forEach(d=>d());
		this._password.dispose();
		this.myBindings?.dispose();
	}
}
