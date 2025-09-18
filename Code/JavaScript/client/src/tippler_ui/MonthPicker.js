import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import E from '../ui_core/E';
import OrderedItems from './OrderedItems';
import Entry from './MonthPicker_Entry';
export default class MonthPicker{
		constructor({model, propertyNameValue, propertyNameVisible}) {
			this._model = model;
			this._element = E.div('month-picker');
			this._valueChanged = this._valueChanged.bind(this);
			this._visibleChanged = this._visibleChanged.bind(this);
			this._handleCallbackSelected = this._handleCallbackSelected.bind(this);
			this.dispose = this.dispose.bind(this);
			this._entries=[];
			const monthStrings = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
			for(var i=0; i<monthStrings.length; i++){
				const monthString = monthStrings[i];
				const selected = this._value ===i;
				const entry = new Entry({monthString, selected, callbackSelected:this._handleCallbackSelected, value:i});
				this._entries.push(entry);
				this._element.appendChild(entry.element);
			}
			this._propertyBindingValue = PropertyBindingFactory.standard(this, model, propertyNameValue, this._valueChanged);
			if(propertyNameVisible)
				this._propertyBindingVisible= PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
			
		}
		get element(){
			return this._element;
		}
		_valueChanged(value){
			if(this._currentlySelectedEntry)
				this._currentlySelectedEntry.selected=false;
			if(isNullOrUndefined(value)||isNaN(value)){
				this._currentlySelectedEntry=null;
				return;
			}
			this._currentlySelectedEntry = this._entries[value];
			this._currentlySelectedEntry.selected=true;
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		_handleCallbackSelected(value){
			this._propertyBindingValue.set(value);
		}
		dispose(){
			this._propertyBindingValue.dispose();
			this._propertyBindingVisible?.dispose();
		}
    }