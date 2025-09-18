import eventEnable from '../core/eventEnable';
import groupFunctions from '../core/groupFunctions';
import ValidityIndicator from './ValidityIndicator';
import Once from '../core/Once';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
export default class TextBox{
    constructor(props) {
        eventEnable(this);
        const {className, classNames, placeholder, isPassword, isTextArea, 
			readOnly, validate, propertyName, model, autoHeight, 
			propertyNameDisabled, propertyNameReadOnly, methodNameOnEnter,
			singleLine, autoWidth, disableOnReadonly, isContentEditable, maxLength}=props;
			this._disableOnReadOnly = disableOnReadonly!==false;
		this._autoHeight = autoHeight;
		this._autoWidth = autoWidth;
		this._disabled = false;
		this._isContentEditable=isContentEditable;
		this.focus = this.focus.bind(this);
		this.dispose = this.dispose.bind(this);
		this._resizeTextareaHeight = this._resizeTextareaHeight.bind(this);
		this._resizeTextareaWidth = this._resizeTextareaWidth.bind(this);
		this._dispatchResized = this._dispatchResized.bind(this);
		this._readOnlyChanged = this._readOnlyChanged.bind(this);
		this._disabledChanged = this._disabledChanged.bind(this);
		this._updateDisabled = this._updateDisabled.bind(this);
		this._valueChanged = this._valueChanged.bind(this);
		this._set = this._set.bind(this);
		this._onceChange = new Once(this._set);
		this.resize = this.resize.bind(this);
		this._textChanged = this._textChanged.bind(this);
		this._setValue= this._setValue.bind(this);
		if(isTextArea){
			this._element = document.createElement('textarea');
			this._element.style.resize = 'none';
			if(singleLine){
				this._element.wrap='off';
				if(autoWidth){
				}
			}
		}
		else if(isContentEditable){
			this._element = document.createElement('div');
			this._element.contentEditable = true;
			this._element.style.resize = 'none';
			if(singleLine){
				this._element.wrap='off';
				if(autoWidth){
				}
			}
		}
		else{
			this._element = document.createElement('input');
			this._element.type =isPassword? 'password':'text';
		}
		if(maxLength){
			this._element.maxLength = maxLength;
		}
		this._additionalSizeRequired=0;
		if(autoHeight){
			this._element.setAttribute('style', 'height:' + (this._element.scrollHeight) + 'px;overflow-y:hidden;');
			this._element.addEventListener("input", this._resizeTextareaHeight);
			this._valueChanged = groupFunctions(this._valueChanged, this._resizeTextareaHeight);
			setTimeout(this._resizeTextareaHeight,0);
		}
		if(autoWidth){
		this._additionalSizeRequired=2;
			this._element.setAttribute('style', 'width:' + (this._element.scrollWidth) + 'px;overflow-x:hidden;');
			this._element.addEventListener("input", this._resizeTextareaWidth);
			this._valueChanged = groupFunctions(this._valueChanged, this._resizeTextareaWidth);
			setTimeout(this._resizeTextareaWidth,0);
		}
        if (readOnly)
            this._readOnlyChanged(true);
        if (className)
            this._element.classList.add(className);
        if (classNames)classNames.forEach((className)=>{
            this._element.classList.add(className);
        });
        if (placeholder)
            this._element.placeholder = placeholder;
        this._element.classList.add('text-box');
		if(methodNameOnEnter){
			this._element.addEventListener("keydown", function(e) {
				e=e||window.event;
				if (e.keyCode === 13){
					model[methodNameOnEnter]();
					e.preventDefault()
				}
			});
		}
		if(validate){
			this._validityIndicator = new ValidityIndicator(this, model, propertyName, this._element);
		}
        this._propertyBinding = PropertyBindingFactory.standard(this, model, propertyName, this._valueChanged);
		if(propertyNameReadOnly){
			this._propertyBindingReadOnly = PropertyBindingFactory.standard(this, model, propertyNameReadOnly, this._readOnlyChanged);
		}
		var propertyBindingDisabled;
		if(propertyNameDisabled){
			this._propertyBindingDisabled = PropertyBindingFactory.standard(this, model, propertyNameDisabled, this._disabledChanged);
		}
		this._element.addEventListener('input', this._textChanged);
	}
	get element() { 
		return this._element; 
	}
	get value(){
		return this._isContentEditable?this._element.innerHTML:this._element.value;
	}
	_setValue(value){
		if(this._isContentEditable){
			this._element.innerHTML = value;
			return;
		}
		this._element.value = value;
	}
	get validityIndicator(){
		return this._validityIndicator;
	}
	set readOnly(value){
		this._readOnlyChanged(value);
	}
	resize(){
		if(this._autoHeight)
			this._resizeTextareaHeight();
		if(this._autoWidth)
			this._resizeTextareaWidth();
	}
    focus() {
		this._element.focus();
	}
	dispose(){
		this.myBindings&&this.myBindings.dispose();
	}
	_resizeTextareaHeight() {
		const parentElement = this._element.parentElement;
		if(!parentElement)return;
		const scrollTop = parentElement.scrollTop;
		this._element.style.height = 'auto';
		this._element.style.height = (this._element.scrollHeight+this._additionalSizeRequired) + 'px'; 
		parentElement.scrollTop = scrollTop;
		this._dispatchResized();
		
	}
	_resizeTextareaWidth() {
		this._element.style.width = '0px';
		this._element.style.width = (this._element.scrollWidth+this._additionalSizeRequired) + 'px'; 
		this._dispatchResized();
		
	}
	_readOnlyChanged(value){
		if(this._readOnly===value)return;
		this._readOnly = value;
		this._element.readOnly = value;
		if(value){
			this._element.classList.add('readonly');
			document.activeElement.blur();
		}
		else{
			this._element.classList.remove('readonly');
		}
		this._updateDisabled();
	}
	_disabledChanged(value){
		if(this._disabled===value)return;
		this._disabled = value;
		this._updateDisabled();
	}
	_updateDisabled(){
		this._element.disabled=this._disabled|| (this._readOnly&&this._disableOnReadOnly)?true:false;
	}
	_valueChanged(value) {
		if (value === undefined || value === null)
			value = '';
		if(this.value===value)return;
		this._setValue(value);
	}
	_textChanged() {
		this._onceChange.trigger();
	}
	_set(){
		this._propertyBinding.set(this.value);
	}
	_dispatchResized(){
		this.dispatchEvent({type:'resized'});
	}
	/*
    _addEventListener(name, callback) {
            switch (name) {
                case 'keydown':
                case 'keyup':
                case 'click':
                    this._element.addEventListener(name, callback);
                    break;
				default:
					//_addEventListener(name, callback);
				break;
            }
        }
    };*/
}