import isNullOrUndefined from '../core/isNullOrUndefined';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import E from '../ui_core/E';
import DateTimeHelper from '../core/DateTimeHelper';
import _createButton from '../ui_core/_createButton';
import DatePicker from './DatePicker';
export default class DateTimePickerCompact{
		constructor({
			model, 
			propertyNameValue, 
			propertyNameVisible, 
			propertyNameDatePickerVisible, 
			propertyNameTimePickerVisible
		}) {
			this._model = model;
			this._disposes = [];
			this._element = E.div('date-time-picker-compact');
			this._valueChanged = this._valueChanged.bind(this);
			this._visibleChanged = this._visibleChanged.bind(this);
			this._showDatePicker = this._showDatePicker.bind(this);
			this._showTimePicker = this._showTimePicker.bind(this);
			this._datePickerVisibleChanged = this._datePickerVisibleChanged.bind(this);
			this._timePickerVisibleChanged = this._timePickerVisibleChanged.bind(this);
			this.dispose = this.dispose.bind(this);
			this._setDate = this._setDate.bind(this);
			this._setTime = this._setTime.bind(this);
			this._buttonDate = _createButton('Date', 'button-date', this._showDatePicker, this._disposes);
			//this._buttonDate.innerHTML='Date';
			this._buttonTime = _createButton('Time', 'button-time', this._showTimePicker, this._disposes);
			//this._buttonTime.innerHTML='Time';
			this._element.appendChild(this._buttonDate);
			this._element.appendChild(this._buttonTime);
			this._propertyBindingValue = PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
			if(propertyNameVisible)
				this._propertyBindingVisible = PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
			this._propertyBindingDatePickerVisible = PropertyBindingFactory.standard(this, model, propertyNameDatePickerVisible, this._datePickerVisibleChanged);
			this._propertyBindingTimePickerVisible = PropertyBindingFactory.standard(this, model, propertyNameTimePickerVisible, this._timePickerVisibleChanged);
		}
		get element(){
			return this._element;
		}
		_valueChanged(value){
			if(isNullOrUndefined(value))
			{
				this._setDate(null);
				this._setTime(null);
				return;
			}
			const date = new Date(value);
			this._setDate(date);
			this._setTime(date);
		}
		_setDate(date){
			if(isNullOrUndefined(date)){
				this._buttonDate.textContent = 'Date';
				return;
			}
			this._buttonDate.textContent = DateTimeHelper.getDateStringWithNumbersFromDate(date);
		}
		_setTime(date){
			if(isNullOrUndefined(date)){
				this._buttonTime.textContent = 'Time';
				return;
			}
			this._buttonTime.textContent = DateTimeHelper.getTimeStringFromDate(date);
		}
		_showDatePicker(){
			this._propertyBindingDatePickerVisible.set(true);
		}
		_showTimePicker(){
			this._propertyBindingTimePickerVisible.set(true);
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		_datePickerVisibleChanged(){
			
		}
		_timePickerVisibleChanged(){
			
		}
		dispose(){
			this._disposes.forEach(d=>d());
			this.myBindings?.dispose();
		}
    }