import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import E from '../ui_core/E';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import OrderedItems from './OrderedItems';
export default class DaysOfTheWeekPicker_Day{
		constructor({dayString, callbackSelectedChanged}){
			this._element = E.div('day');
			this._element.textContent = dayString;
			
			this._selected = false;
			this._element.addEventListener('click', ()=>{
				this.selected =! this.selected;
				callbackSelectedChanged()
			});
		}
		get element(){
			return this._element;
		}
		get selected(){
			return this._selected;
		}
		set selected(value){
			this._selected = value;
			if(value)
				this._element.classList.add('selected');
			else
				this._element.classList.remove('selected');
		}
}