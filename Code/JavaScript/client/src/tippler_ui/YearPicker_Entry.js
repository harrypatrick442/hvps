import _createButton from '../ui_core/_createButton';
import E from '../ui_core/E';
export default class YearPicker_Entry{
		constructor({callbackClicked, disposes}){
			this._element = E.div('entry');
			this._innerElement = _createButton('', 'inner',  ()=>callbackClicked(this._value), disposes);
			this._element.appendChild(this._innerElement);
		}
		get element(){
			return this._element;
		}
		set value(value){
			this._value = value;
			this._innerElement.textContent = String(value);
		}
		set selected(value){
			if(value)
				this._element.classList.add('selected');
			else
				this._element.classList.remove('selected');
		}
}