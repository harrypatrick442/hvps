import i from '../ui_core/i';
import isNullOrUndefined from '../core/isNullOrUndefined';
import E from '../ui_core/E';
import DateTimePickerCompact from './DateTimePickerCompact';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import ClickedOffManager from '../ui_core/ClickedOffManager';
import DatePicker from './DatePicker';
import TimePicker from './TimePicker';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import './DateTimePicker.css';
export default class DateTimePicker{
		constructor({
			model, 
			propertyNameValue, 
			propertyNameVisible, 
			propertyNameDatePickerVisible, 
			propertyNameTimePickerVisible,
			hideOnClickedOff
		}) {
			const disposes = [];
			this._disposes = disposes;
			this._model = model;
			this._hide = this._hide.bind(this);
			this.dispose = this.dispose.bind(this);
			this._visibleChanged = this._visibleChanged.bind(this);
			this._datePickerVisibleChanged = this._datePickerVisibleChanged.bind(this);
			this._timePickerVisibleChanged = this._timePickerVisibleChanged.bind(this);
			this._element = E.div('date-time-picker');
			this._dateTimePicker = new DateTimePickerCompact({
				model, 
				propertyNameValue, 
				propertyNameVisible, 
				propertyNameDatePickerVisible, 
				propertyNameTimePickerVisible
			});
			this._datePicker = new DatePicker({
				model, 
				propertyNameValue,
				propertyNameVisible:propertyNameDatePickerVisible
			});
			this._timePicker = new TimePicker({
				model, 
				propertyNameValue,
				propertyNameVisible:propertyNameTimePickerVisible
			});
			if(hideOnClickedOff)
				this._clickedOffManager = new ClickedOffManager({
					element:this._element, hide:this._hide});
			this._propertyBindingVisible = PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
			PropertyBindingFactory.standard(this, model, propertyNameDatePickerVisible, this._datePickerVisibleChanged);
			PropertyBindingFactory.standard(this, model, propertyNameTimePickerVisible, this._timePickerVisibleChanged);
			this._closeButton = _createImageHoverButton(i('CloseThickMottledWhite'), i('CloseThickWhite'), 'close-button', this._hide, disposes);
			this._topRowElement= E.div('top-row');
			this._topRowElement.appendChild(this._dateTimePicker.element);
			this._topRowElement.appendChild(this._closeButton);
			this._element.appendChild(this._topRowElement);
			this._element.appendChild(this._datePicker.element);
			this._element.appendChild(this._timePicker.element);
		}
		get element(){
			return this._element;
		}
		_hide(){
			this._propertyBindingVisible.set(false);
		}
		_visibleChanged(value){
			this._clickedOffManager.dispose();
			if(value){
				this._element.classList.add('visible');
				this._clickedOffManager.register();
			}
			else 
				this._element.classList.remove('visible');
		}
		_datePickerVisibleChanged(value){
			if(value)
				this._element.classList.add('picking-date');
			else 
				this._element.classList.remove('picking-date');
		}
		_timePickerVisibleChanged(value){
			if(value)
				this._element.classList.add('picking-time');
			else 
				this._element.classList.remove('picking-time');
		}
		dispose(){
			this._disposes.forEach(d=>d());
			this.myBindings?.dispose();
		}
    }