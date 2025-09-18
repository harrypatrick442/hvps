import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import OrderedItems from './OrderedItems';
import E from '../ui_core/E';
export default class DateTimePicker_DayInMonth{
		constructor({selected, callbackSelected, value}){
			this._element = E.div('day-in-month');
			this._element.textContent = String(value);
			this._selected = selected;
			this._element.addEventListener('click', ()=>callbackSelected(this));
		}
		get value(){
			return this._value;
		}
		set selected(value){
			if(value)
				this._element.classList.add('selected');
			else
				this._element.classList.remove('selected');
		}
}