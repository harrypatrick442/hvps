import isNullOrUndefined from '../core/isNullOrUndefined';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import E from '../ui_core/E';
import DateTimeHelper from '../core/DateTimeHelper';
import _createButton from '../ui_core/_createButton';
import UpDown from './TimePicker_UpDown';
import './TimePicker.css';
export default class TimePicker{
		constructor({model, propertyNameValue, propertyNameVisible}) {
			this._model = model;
			this._element = E.div('time-picker');
			this._valueChanged = this._valueChanged.bind(this);
			this._visibleChanged = this._visibleChanged.bind(this);
			this._handleChanged=this._handleChanged.bind(this);
			this._createColon=this._createColon.bind(this);
			this._upDownHours = new UpDown({callbackChanged:this._handleChanged, maxValueInclusive:23});
			this._upDownMinutes = new UpDown({callbackChanged:this._handleChanged, maxValueInclusive:59});
			this._colon2 = E.div('colon');
			this._colon2.textContent = ':';
			this._upDownSeconds = new UpDown({callbackChanged:this._handleChanged, maxValueInclusive:59});
			[this._upDownHours.element, this._createColon(), this._upDownMinutes.element, this._createColon(), this._upDownSeconds.element]
				.forEach(e=>this._element.appendChild(e));
			this._propertyBindingValue = PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
			if(propertyNameVisible)
				this._propertyBindingVisible = PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
		}
		get element(){
			return this._element;
		}
		_createColon(){
			const colonElement = E.div('colon');
			const innerElement = E.div('inner');
			innerElement.textContent = ':';
			colonElement.appendChild(innerElement);
			return colonElement;
		}
		_valueChanged(value){
			if(isNullOrUndefined(value)||isNaN(value)){
				return;
			}
			const date = new Date(value);
			const seconds = date.getSeconds();
			const a = (value%3600);
			const minutes = date.getMinutes();
			const hours = date.getHours();
			this._upDownHours.value = hours;
			this._upDownMinutes.value = minutes;
			this._upDownSeconds.value = seconds;
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		_handleChanged(){
			let millisecondsUTC = this._propertyBindingValue.get();
			const hadNoMillisecondsUTC = isNullOrUndefined(millisecondsUTC);
			const date = hadNoMillisecondsUTC ?new Date():new Date(millisecondsUTC) ;
			date.setSeconds(this._upDownSeconds.value);
			date.setMinutes(this._upDownMinutes.value);
			date.setHours(this._upDownHours.value);
			millisecondsUTC = hadNoMillisecondsUTC ?date.getTime()%86400000:date.getTime();
			this._propertyBindingValue.set(millisecondsUTC);
		}
		dispose(){
			this.myBindings?.dispose();
		}
    }