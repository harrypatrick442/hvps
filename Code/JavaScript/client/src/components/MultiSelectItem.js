import E from '../ui_core/E';
export default class MultiSelectOption{
    constructor ({text, value, selectedChanged, callbackSetFocused}) {
		this._value = value;
		this._text = text;
		this._selectedChanged = selectedChanged;
		this._handleClick = this._handleClick.bind(this);
		this.focus = this.focus.bind(this);
		this.dispose = this.dispose.bind(this);
		this._selected = false;
		this._element = E.button('item');
		this._element.innerHTML = text;
		this._element.addEventListener('click', this._handleClick);
		const focused = ()=>callbackSetFocused(this);
		this._element.addEventListener('focus', focused);
		this._disposes = [()=>this._element.removeEventListener('focus', focused)];
	}
	get value(){
		return this._value;
	}
	get text(){
		return this._text;
	}
	get selected(){
		return this._selected;
	}
	set selected(value){
		if(this._selected === value)return;
		this._selected = value;
		if(value)
			this._element.classList.add('selected');
		else
			this._element.classList.remove('selected');
		this._selectedChanged(this);
	}
	get element(){
		return this._element;
	}
	focus(){
		this._element.focus();
	}
	_handleClick(){
		this.selected = !this._selected;
		this._selectedChanged(this, true);
	}
	dispose(){
		this._disposes.forEach(d=>d());
		this._element.removeEventListener('mousedown', this._handleClick);
	}
}