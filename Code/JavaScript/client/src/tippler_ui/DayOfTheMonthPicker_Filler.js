import _createButton from '../ui_core/_createButton';
import E from '../ui_core/E';
export default class DayOfTheMonthPicker_Filler{
		constructor({disposes}){
			this._element = E.div('filler');
			this._innerElement = _createButton('', 'inner',  ()=>{}, disposes);
			this._innerElement.disabled = true;
			this._element.appendChild(this._innerElement);
		}
		get element(){
			return this._element;
		}
		set dayOfMonth(value){
			this._innerElement.textContent = String(value);
		}
		set visible(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
}