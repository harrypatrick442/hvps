import isNullOrUndefined from '../core/isNullOrUndefined';
import eventEnable from '../core/eventEnable';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import DayInMonth from './DatePicker_DayInMonth';
import E from '../ui_core/E';
import exposeBinding from '../mvvm/exposeBinding';
import DateTimeHelper from '../core/DateTimeHelper';
import _createButton from '../ui_core/_createButton';
import MonthPicker from './MonthPicker';
import EditableTextField from './EditableTextField';
import DayOfTheMonthPicker from './DayOfTheMonthPicker';
import YearPicker from './YearPicker';
import DayOfTheMonthPicker_Entry from './DayOfTheMonthPicker_Entry';
import DayOfTheMonthPicker_Filler from './DayOfTheMonthPicker_Filler';
import './DatePicker.css';
export default class DatePicker{
		constructor({model, propertyNameValue, propertyNameVisible,
		createDayOfTheMonthPickerEntry, createDayOfTheMonthPickerFiller}) {
			createDayOfTheMonthPickerEntry ||= (params)=>new DayOfTheMonthPicker_Entry(params);
			createDayOfTheMonthPickerFiller ||= (params)=>DayOfTheMonthPicker_Filler(params);
			this._model = model;
			this._disposes= [];
			this._element = E.div('date-picker');
			this._dayMonthYearRow = E.div('day-month-year-row');
			this._valueChanged = this._valueChanged.bind(this);
			this._visibleChanged = this._visibleChanged.bind(this);
			this._handleClickDayButton = this._handleClickDayButton.bind(this);
			this._handleClickMonthButton = this._handleClickMonthButton.bind(this);
			this._handleClickYearButton = this._handleClickYearButton.bind(this);
			this._element.appendChild(this._dayMonthYearRow);
			this._dayButton= _createButton('Day', 'date-button', this._handleClickDayButton, this._disposes);
			this._dayMonthYearRow.appendChild(this._dayButton);
			this._monthButton= _createButton('Month', 'date-button', this._handleClickMonthButton, this._disposes);
			this._dayMonthYearRow.appendChild(this._monthButton);
			this._yearButton= _createButton('Year', 'date-button', this._handleClickYearButton, this._disposes);
			this._dayMonthYearRow.appendChild(this._yearButton);
			this.__daysOfTheMonthVisible=true;
			this.__monthsVisible = false;
			this.__yearsVisible = false;
			exposeBinding(this, '_yearsVisible', ()=>this._yearsVisible, (value)=>this._yearsVisible=value);
			exposeBinding(this, '_month', ()=>this._month, (value)=>this._month=value);
			exposeBinding(this, '_monthsVisible', ()=>this._monthsVisible, (value)=>this._monthsVisible=value);
			exposeBinding(this, 'millisecondsUTC', ()=>this.millisecondsUTC, (value)=>this.millisecondsUTC=value);
			exposeBinding(this, '_daysOfTheMonthVisible', ()=>this._daysOfTheMonthVisible, (value)=>this._daysOfTheMonthVisible=value);
			this._propertyBindingValue =  PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged, true);
			this._propertyBindingVisible = PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
			this._monthPicker = new MonthPicker({model:this, propertyNameValue:'_month', propertyNameVisible:'_monthsVisible'});
			this._yearPicker = new YearPicker({model:this, propertyNameMillisecondsUTC:'millisecondsUTC',
				propertyNameVisible:'_yearsVisible'});
			this._dayOfTheMonthPicker = new DayOfTheMonthPicker({
				model:this, propertyNameMillisecondsUTC:'millisecondsUTC',
				propertyNameVisible:'_daysOfTheMonthVisible', createDayOfTheMonthPickerEntry, createDayOfTheMonthPickerFiller});
			this._element.appendChild(this._dayOfTheMonthPicker.element);
			this._element.appendChild(this._monthPicker.element);
			this._element.appendChild(this._yearPicker.element);
			this._valueChanged(this._propertyBindingValue.get());
			
		}
		get element(){
			return this._element;
		}
		get millisecondsUTC(){
			return this._propertyBindingValue.get();
		}
		set millisecondsUTC(value){
			return this._propertyBindingValue.set(value);
		}
		get _daysOfTheMonthVisible(){
			return this.__daysOfTheMonthVisible;
		}
		set _daysOfTheMonthVisible(value){
			if(this.__daysOfTheMonthVisible===value)return;
			this.__daysOfTheMonthVisible = value;
			this.bindingsHandler.changed('_daysOfTheMonthVisible', value);
		}
		get _month(){
			return new Date(this.millisecondsUTC).getMonth();
		}
		set _month(value){
			const date = new Date(this.millisecondsUTC);
			date.setMonth(value);
			const newMillisecondsUTC = date.getTime();
			if(this.millisecondsUTC===newMillisecondsUTC)return;
			this.millisecondsUTC = newMillisecondsUTC;
		}
		get _monthsVisible(){
			return this.__monthsVisible;
		}
		set _monthsVisible(value){
			if(this.__monthsVisible===value)return;
			this.__monthsVisible=value;
			this.bindingsHandler.changed('_monthsVisible', value);
		}
		get _yearsVisible(){
			return this.__yearsVisible;
		}
		set _yearsVisible(value){
			if(this.__yearsVisible===value)return;
			this.__yearsVisible=value;
			this.bindingsHandler.changed('_yearsVisible', value);
		}
		set _yearValid(value){
			if(value)
				this._editableTextFieldYear.element.classList.add('valid');
			else
				this._editableTextFieldYear.element.classList.remove('valid');
		}
		get _yearText(){
			return this._yearText;
		}
		set _yearText(value){
			return;
			if(this.__yearText===value)return;
			this.__yearText=value;
			this.bindingsHandler.changed('_yearText', value);
			const intValue = parseInt(value);
			const isValid = !isNaN(intValue);
			this._yearValid= isValid;
			if(!isValid)return;
			const currentDate = new Date(this.millisecondsUTC);
			currentDate.setYear(this.intValue);
			this._propertyBindingValue.set(currentDate.getTime());
		}
		_valueChanged(value){
			if(isNullOrUndefined(value))
			{
				return;
			}
			const date = new Date(value);
			this._yearButton.textContent = String(date.getYear()+1900);
			this._monthButton.textContent = DateTimeHelper.getMonthString(date.getMonth(), true);
			const dayOfMonth = date.getDate();
			this._dayButton.textContent = dayOfMonth+DateTimeHelper.getDayOfMonthSuffix(dayOfMonth);
			this.bindingsHandler.changed('millisecondsUTC', value);
			this.bindingsHandler.changed('_month', this._month);
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		_handleClickDayButton(){
			this._daysOfTheMonthVisible =true;
			this._monthsVisible = false;
			this._yearsVisible = false;
		}
		_handleClickMonthButton(){
			this._daysOfTheMonthVisible =false;
			this._monthsVisible = true;
			this._yearsVisible = false;
		}
		_handleClickYearButton(){
			this._daysOfTheMonthVisible =false;
			this._monthsVisible = false;
			this._yearsVisible = true;
		}
		dispose(){
			this._disposes.forEach(d=>d());
			this.myBindings?.dispose();
		}
    }