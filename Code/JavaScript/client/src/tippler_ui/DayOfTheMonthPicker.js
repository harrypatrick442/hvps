import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import DateTimeHelper from '../core/DateTimeHelper';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import E from '../ui_core/E';
import OrderedItems from './OrderedItems';
import DayOfTheMonthPicker_Entry from './DayOfTheMonthPicker_Entry';
import DayOfTheMonthPicker_Filler from './DayOfTheMonthPicker_Filler';
export default class DayOfTheMonthPicker{
		constructor({model, propertyNameMillisecondsUTC, propertyNameVisible, 
			createDayOfTheMonthPickerEntry, createDayOfTheMonthPickerFiller}) {
			this._model = model;
			this._disposes = [];
			this._element = E.div('day-of-the-month-picker');
			this._visibleChanged = this._visibleChanged.bind(this);
			this._millisecondsUTCChanged = this._millisecondsUTCChanged.bind(this);
			this._handleClickedEntry = this._handleClickedEntry.bind(this);
			this._headingsElement = E.div('headings');
			this._element.appendChild(this._headingsElement);
			this.dispose = this.dispose.bind(this);
			for(var i=0; i<7; i++){
				const headingElement = E.div('heading');
				headingElement.textContent = DateTimeHelper.getDayOfWeekString(i, true);
				this._headingsElement.appendChild(headingElement);
			}
			this._fillers = [];
			this._entries=[];
			for(var i=0; i<6; i++){
				const filler = createDayOfTheMonthPickerFiller({disposes:this._disposes, index:i});
				this._fillers.push(filler);
				this._element.appendChild(filler.element);
			}
			for(var i=1; i<32; i++){
				const entry = createDayOfTheMonthPickerEntry({
					value:i, 
					callbackClicked:this._handleClickedEntry, 
					disposes:this._disposes,
					index:i-1
				});
				this._entries.push(entry);
				this._element.appendChild(entry.element);
			}
			this._propertyBindingMillisecondsUTC = PropertyBindingFactory.standard(this, model, propertyNameMillisecondsUTC, this._millisecondsUTCChanged);
			if(propertyNameVisible)
				this._propertyBindingVisible= PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
		}
		
		get element(){
			return this._element;
		}
		_millisecondsUTCChanged(value){
			if(this._currentlySelectedEntry){
				this._currentlySelectedEntry.selected=false;
				this._currentlySelectedEntry=null;
			}
			if(isNullOrUndefined(value))
			{
				return;
			}
			const date = new Date(value);
			const dayOfMonth  = date.getDate();
			const daysInMonth = DateTimeHelper.daysInMonthFromDate(date);
			this._updateEntriesVisible(daysInMonth, date);
			const entryForDay = this._entries[dayOfMonth-1];
			this._currentlySelectedEntry = entryForDay;
			this._currentlySelectedEntry.selected=true;
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		_handleClickedEntry(dayOfMonth){
			const millisecondsUTC = this._propertyBindingMillisecondsUTC.get();
			const date = isNullOrUndefined(millisecondsUTC)?new Date():new Date(millisecondsUTC);
			date.setDate(dayOfMonth);
			this._propertyBindingMillisecondsUTC.set(date.getTime());
		}
		_updateEntriesVisible(daysInMonth, date){
			const daysInPreviousMonth = DateTimeHelper.getDaysInPreviousMonthFromDate(date);
			const dayOfWeekIndexForFirstDayOfMonth = DateTimeHelper.getDayOfWeekForFirstDayOfMonth(date)-1;
			var firstFillerDayOfMonth = 1+daysInPreviousMonth - dayOfWeekIndexForFirstDayOfMonth;
			let dayInLastMonth = daysInPreviousMonth;
			const fillers = this._fillers;
			let j= fillers.length - 1;
			for(var i=dayOfWeekIndexForFirstDayOfMonth; i>0; i--){
				const filler = fillers[j--];
				filler.visible=true;
				filler.dayOfMonth = dayInLastMonth--;
			}
			while(j>=0){
				this._fillers[j--].visible=false;
			}
			for(var i = this._entries.length-1; i>=28; i--){
				const visible = i<daysInMonth;
				const entry = this._entries[i];
				entry.visible = visible;
			} 
		}
		dispose(){
			this._disposes.forEach(d=>d());
			this.myBindings.dispose();
		}
    }