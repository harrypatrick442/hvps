import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import OrderedItems from './OrderedItems';
import E from '../ui_core/E';
import DaysOfTheWeekPicker_Day from './DaysOfTheWeekPicker_Day';
export default class DaysOfTheWeekPicker{
	constructor({model, propertyNameValue, propertyNameVisible}) {
		this._model = model;
		this._element = E.div('days-of-the-week-picker');
		this._valueChanged = this._valueChanged.bind(this);
		this._visibleChanged = this._visibleChanged.bind(this);
		this._handleSelectedChanged = this._handleSelectedChanged.bind(this);
		this._getFlagFromEntries = this._getFlagFromEntries.bind(this);
		this._setEntriesFromFlag = this._setEntriesFromFlag.bind(this);
		this.dispose = this.dispose.bind(this);
		this._entries=[];
		const dayStrings = ['Mon', 'Tue', 'Web', 'Thu', 'Fri', 'Sat', 'Sun'];
		dayStrings.forEach(dayString=>{
			const entry = new DaysOfTheWeekPicker_Day({dayString, callbackSelectedChanged:this._handleSelectedChanged});
			this._entries.push(entry);
			this._element.appendChild(entry.element);
		});
		this._propertyBindingValue = PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
		if(propertyNameVisible)
			this._propertyBindingVisible= PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
		
	}
	_valueChanged(value){
		this._setEntriesFromFlag(value);
	}
	_visibleChanged(value){
		if(value)
			this._element.classList.add('visible');
		else
			this._element.classList.remove('visible');
	}
	_handleSelectedChanged(entry){
		const flag = this._getFlagFromEntries();
		this._propertyBindingValue.set(flag);
	}
	_getFlagFromEntries(){
		let multiplier = 1;
		let value =0;
		for(var i=0; i<7; i++){
			value +=(this._entries[i].selected?1:0)*multiplier;
			multiplier*=2;
		}
		return value;
	}
	_setEntriesFromFlag(){
		let multiplier = 1;
		for(var i=0; i<7; i++){
			this._entries[i].selected=multiplier&i;
			multiplier*=2;
		}
	}
	dispose(){
		this._propertyBindingValue.dispose();
		this._propertyBindingVisible?.dispose();
	}
}