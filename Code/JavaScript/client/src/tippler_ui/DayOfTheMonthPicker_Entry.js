import _createButton from '../ui_core/_createButton';
import E from '../ui_core/E';
export default class DayOfTheMonthPicker_Entry{
		constructor({value, callbackClicked, disposes}){
			this._element = E.div('day');
			this._innerElement = _createButton(String(value), 'inner',  ()=>callbackClicked(value), disposes);
			this._element.appendChild(this._innerElement);
		}
		get element(){
			return this._element;
		}
		set selected(value){
			if(value)
				this._element.classList.add('selected');
			else
				this._element.classList.remove('selected');
		}
}