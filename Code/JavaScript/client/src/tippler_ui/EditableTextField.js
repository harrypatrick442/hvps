import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import TextBox from './TextBox';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import ClickedOffManager from '../ui_core/ClickedOffManager';
import './EditableTextField.css';
import isNullOrUndefined from '../core/isNullOrUndefined';
import eventEnable from '../core/eventEnable';
import E from '../ui_core/E';
import i from '../ui_core/i';
import LoopBreak from '../core/LoopBreak';
export default class EditableTextField{
	constructor({model, propertyName, placeholder, editOnClickText, multiLine, 
		separateTextElement, validate, methodNameFinishedEditing, icon}){
		this._disposes =[];
		if(isNullOrUndefined(multiLine))
			multiLine = false;
		eventEnable(this);
		this._editOnClickText = editOnClickText;
		this._handleFocusOut= this._handleFocusOut.bind(this);
		this._handleClickedText= this._handleClickedText.bind(this);
		this._clickedEditButton= this._clickedEditButton.bind(this);
		this._edit= this._edit.bind(this);
		this._dispatchResized = this._dispatchResized.bind(this);
		this._dispatchFinishedEdit = this._dispatchFinishedEdit.bind(this);
		this.dispose= this.dispose.bind(this);
		this._setEditing= this._setEditing.bind(this);
		this._focussed= this._focussed.bind(this);
		this._element = document.createElement('div');
		this._element.classList.add('editable-text-field');
		//this._loopBreakFocusOut = new LoopBreak();
		this._textBox = new TextBox({ placeholder, 
			isPassword:false, readOnly:false,
			propertyName, model, singleLine:!multiLine, isTextArea:multiLine, validate, autoWidth:false, autoHeight:false, disableOnReadonly:editOnClickText?false:true});
		this._textBox.addEventListener('resized', this._dispatchResized);
		this._textBox.element.addEventListener('mousedown', this._handleClickedText);
		this._textBox.element.addEventListener('focus', this._focussed);
		this._editButton = _createImageHoverButton(i('Edit'), i('EditHover'), 'edit-button', this._clickedEditButton, this._disposes);
		this._editButton.addEventListener('focus', this._focussed);
		this._element.appendChild(this._textBox.element);
		this._element.appendChild(this._editButton);
		if(separateTextElement){
			this._textElement = E.div('text');
			this._textElement.addEventListener('mousedown', this._handleClickedText);
			this._element.classList.add('separate-text-element');
			PropertyBindingFactory.standard(this, model, propertyName, (value)=>this._textElement.textContent = value);
			this._element.appendChild(this._textElement);
		}
		this._clickedOffManager = new ClickedOffManager({element:this._element, hide:()=>this._setEditing(false)});
		this._textBox.element.addEventListener('focusout', this._handleFocusOut);
		if(methodNameFinishedEditing)
			this._methodFinishedEditing = model[methodNameFinishedEditing];
		
		this._currentlyEditing = false;
		this._textBox.readOnly = true;
		this._element.classList.remove('editing');
		this._element.classList.add('not-editing');
	}
	get element(){
		return this._element;
	}
	get editButton(){
		return this._editButton;
	}
	get desiredSize(){
		return [this.requiredWidthToShowAllText, this.requiredHeightToShowAllText];
	}
	get requiredWidthToShowAllText(){
		return this._editButton.scrollWidth+this._textBox.element.scrollWidth;
	}
	get requiredHeightToShowAllText(){
		return this._element.scrollHeight;
	}
	_dispatchResized(){
		this.dispatchEvent({type:'resized'});
	}
	_handleFocusOut(){
		//this._loopBreakFocusOut.trigger();
		this._setEditing(false);
	}
	_handleClickedText(){
		console.log('clicked text');
		console.log(this._editOnClickText);
		if(this._editOnClickText)
			this._edit();
	}
	_clickedEditButton(){
		this._edit();
	}
	_edit(){
		this._setEditing(true);
		this._clickedOffManager.register();
		setTimeout(()=>
			this._textBox.element.focus());
	}
	_setEditing(value){
		console.log('_setEditing');
		console.log(value);
		console.log(new Error().stack);
		if(this._currentlyEditing===value)return;
		this._currentlyEditing = value;
		this._textBox.readOnly = !value;
		if(value){
			this._element.classList.add('editing');
			this._element.classList.remove('not-editing');
		}
		else{
			this._element.classList.remove('editing');
			this._element.classList.add('not-editing');
		}
		this._dispatchResized();
		if(!value){
			this._methodFinishedEditing&&this._methodFinishedEditing();
			this._dispatchFinishedEdit();
		}
	}
	dispose(){
		this.myBindings?.dispose();
		this._disposes.forEach(d=>d());
		this._textBox.dispose();
		this._textBox.removeEventListener('resized', this._dispatchResized);
		this._textBox.element.removeEventListener('mousedown', this._handleClickedText);
		this._textBox.element.removeEventListener('focus', this._focussed);
		this._textElement?.removeEventListener('mousedown', this._handleClickedText);
		this._textBox.element.removeEventListener('focusout', ()=>this._handleFocusOut);
		this._editButton.removeEventListener('focus', this._focussed);
	}
	_dispatchFinishedEdit(){
		this.dispatchEvent({type:'finishededit'});
	}
	_focussed(){
		//if(this._loopBreakFocusOut.triggered)return;
		this._edit();
	}
}