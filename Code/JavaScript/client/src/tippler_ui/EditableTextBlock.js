import i from '../ui_core/i';
import eventEnable from '../core/eventEnable';
import HashBuilder from '../core/HashBuilder';
import isNullOrUndefined from '../core/isNullOrUndefined';
import FlagsHelper from '../helpers/FlagsHelper';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import E from '../ui_core/E';
import ClickedOff from '../ui_core/ClickedOff';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import './EditableTextBlock.css';
export default class EditableTextBlock{
	constructor({model, propertyNameText, methodNameEdit, className}){
		const disposes = [];
		this._disposes = disposes;
		this._edit = model[methodNameEdit];
		this._textChanged= this._textChanged.bind(this);
		this.dispose= this.dispose.bind(this);
		this._element = E.div('editable-text-block');
		if(className)
			this._element.classList.add(className);
		this._textElement = E.div('text');
		this._element.appendChild(this._textElement);
		if(this._edit){
			this._buttonEdit = _createImageHoverButton(i('Edit'), i('EditHover'), 'edit-button', this._edit, disposes);
			this._element.appendChild(this._buttonEdit);
		}
		this._propertyBindingText = PropertyBindingFactory.standard(this, model, propertyNameText, this._textChanged);
	}
	get element(){
		return this._element;
	}
	_textChanged(value){
		this._textElement.textContent = value;
	}
	dispose(){
		this.myBindings?.dispose();
		this._disposes.forEach(d=>d());
	}
}