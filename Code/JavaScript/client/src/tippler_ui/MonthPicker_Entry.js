import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import E from '../ui_core/E';
import OrderedItems from './OrderedItems';
export default class MonthPicker{
		constructor({monthString, selected, callbackSelected, value}) {
			this._value = value;
			this._element = E.div('entry');
			this._innerElement = E.div('inner');
			this._element.appendChild(this._innerElement);
			this._innerElement.textContent = monthString;
			this._element.addEventListener('click', ()=>callbackSelected(value));
			this.selected = selected;
		}
		get element(){
			return this._element;
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