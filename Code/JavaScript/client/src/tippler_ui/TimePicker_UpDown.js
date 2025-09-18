import i from '../ui_core/i';
import E from '../ui_core/E';
import _createImageButton from '../ui_core/_createImageButton';
import createSpeedClickWhenHold from '../ui_core/createSpeedClickWhenHold';
import padZerosToStart from '../core/padZerosToStart';
export default class TimePicker_UpDown{
	constructor({callbackChanged, maxValueInclusive}){
		this._disposes = [];
		this._callbackChanged  = callbackChanged;
		this._maxValueInclusive = maxValueInclusive;
		this._element = E.div('up-down');
		this._value = 0;
		this._valueElement = E.div('value');
		this._handleClickedUp= this._handleClickedUp.bind(this);
		this._handleClickedDown= this._handleClickedDown.bind(this);
		this._updateText= this._updateText.bind(this);
		this._buttonUp =  _createImageButton(i('UpArrow'), 'button-up', this._handleClickedUp, this._disposes, true); 
		this._buttonDown =  _createImageButton(i('DownArrowHover'), 'button-down', this._handleClickedDown, this._disposes, true); 
		this._element.appendChild(this._buttonUp);
		this._element.appendChild(this._valueElement);
		this._element.appendChild(this._buttonDown);
		createSpeedClickWhenHold({
			element:this._buttonUp, 
			delayTillStartMilliseconds:800, 
			minFrequencyInclusive:3, 
			maxFrequencyInclusive:10, 
			delayFromMinToMaxFrequencyMilliseconds:3000,
			callback:this._handleClickedUp,
			disposes:this._disposes
		});
		createSpeedClickWhenHold({
			element:this._buttonDown, 
			delayTillStartMilliseconds:800, 
			minFrequencyInclusive:3, 
			maxFrequencyInclusive:10, 
			delayFromMinToMaxFrequencyMilliseconds:3000,
			callback:this._handleClickedDown,
			disposes:this._disposes
		});
	}
	get element(){
		return this._element;
	}
	get value(){
		return this._value;
	}
	set value(value){
		this._value = value;
		this._updateText();
	}
	_handleClickedUp(){
		if(this._value>=this._maxValueInclusive)
			this._value =0;
		else
			this._value++;
		this._updateText();
		this._callbackChanged();
	}
	_handleClickedDown(){
		if(this._value<=0)
			this._value = this._maxValueInclusive;
		else
			this._value--;
		this._updateText();
		this._callbackChanged();
	}
	_updateText(){
		this._valueElement.textContent = padZerosToStart(String(this._value), 2);
	}
}