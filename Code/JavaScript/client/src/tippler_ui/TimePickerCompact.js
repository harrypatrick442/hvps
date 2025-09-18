import isNullOrUndefined from '../core/isNullOrUndefined';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import E from '../ui_core/E';
export default class TimePickerCompact{
		constructor({model, propertyNameValue, propertyNameVisible}) {
			this._model = model;
			this._element = E.div('time-picker-compact');
			this._valueChanged = this._valueChanged.bind(this);
			this._visibleChanged = this._visibleChanged.bind(this);
			this.dispose = this.dispose.bind(this);
			this._setTime = this._setTime.bind(this);
			this._buttonTime = E.button('button-time');
			this._element.appendChild(this._buttonTime);
			this._propertyBindingValue = PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
			this._propertyBindingVisible = PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
		}
		_valueChanged(value){
			if(isNullOrUndefined(value))
			{
				this._setTime(null);
				return;
			}
			const date = new Date(value);
			this._setTime(date);
		}
		_setTime(date){
			if(isNullOrUndefined(date)){
				this._buttonTime.textContent = '';
				return;
			}
			const hours = date.getHours();
			const minutes = date.getMinutes();
			const seconds = date.getSeconds();
			const timeString = `${hours}:${minutes}:${seconds}`;
			this._buttonTime.textContent = timeString;
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		dispose(){
			this.bindingsHandler.dispose();
		}
    }