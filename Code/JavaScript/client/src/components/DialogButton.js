import E from '../ui_core/E';
import './DialogButton.css';
export default class DialogButton{
	constructor({onClick, text, disposes, className}){
		this.dispose = this.dispose.bind(this);
		this._element = E.button('dialog-button');
		if(className)
			this._element.classList.add(className);
		this._element.addEventListener('click', onClick);
		disposes.push(()=>this._element.removeEventListener('click', onClick));
		this._element.textContent  = text;
	}
	get element(){
		return this._element;
	}
	dispose(){
		const parent = this._element.parentNode;
		if(parent)
			parent.removeChild(this._element);
	}
}