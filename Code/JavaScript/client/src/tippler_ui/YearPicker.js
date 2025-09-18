import i from '../ui_core/i';
import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import DateTimeHelper from '../core/DateTimeHelper';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import E from '../ui_core/E';
import _createImageButton from '../ui_core/_createImageButton';
import createSpeedClickWhenHold from '../ui_core/createSpeedClickWhenHold';
import YearPicker_Entry from './YearPicker_Entry';
const N_YEARS = 20;
export default class YearPicker{
		constructor({model, propertyNameMillisecondsUTC, propertyNameVisible}) {
			this._model = model;
			this._disposes = [];
			this._visibleChanged = this._visibleChanged.bind(this);
			this._millisecondsUTCChanged = this._millisecondsUTCChanged.bind(this);
			this._updateYears = this._updateYears.bind(this);
			this._getYearToStartFrom = this._getYearToStartFrom.bind(this);
			this._handleClickedEntry = this._handleClickedEntry.bind(this);
			this._handleClickedLeft = this._handleClickedLeft.bind(this);
			this._handleClickedRight = this._handleClickedRight.bind(this);
			this._updateEntriesFromYear = this._updateEntriesFromYear.bind(this);
			this.dispose = this.dispose.bind(this);
			this._element = E.div('year-picker');
			this._entriesElement = E.div('entries');
			this._buttonLeft = _createImageButton(i('LeftArrow'), 'button-left', this._handleClickedLeft, this._disposes, true);
			this._buttonRight = _createImageButton(i('RightArrow'), 'button-right', this._handleClickedRight, this._disposes, true);			
			createSpeedClickWhenHold({
				element:this._buttonLeft, 
				delayTillStartMilliseconds:800, 
				minFrequencyInclusive:3, 
				maxFrequencyInclusive:10, 
				delayFromMinToMaxFrequencyMilliseconds:3000,
				callback:this._handleClickedLeft,
				disposes:this._disposes
			});
			
			createSpeedClickWhenHold({
				element:this._buttonRight, 
				delayTillStartMilliseconds:800, 
				minFrequencyInclusive:3, 
				maxFrequencyInclusive:10, 
				delayFromMinToMaxFrequencyMilliseconds:3000,
				callback:this._handleClickedRight,
				disposes:this._disposes
			});
			this._element.appendChild(this._buttonLeft);
			this._element.appendChild(this._entriesElement);
			this._element.appendChild(this._buttonRight);
			this._entries=[];
			for(var j=0; j<N_YEARS; j++){
				const entry = new YearPicker_Entry({
					disposes:this._disposes, 
					callbackClicked:this._handleClickedEntry});
				this._entries.push(entry);
				this._entriesElement.appendChild(entry.element);
			}
			this._propertyBindingMillisecondsUTC = PropertyBindingFactory.standard(this, model, propertyNameMillisecondsUTC, this._millisecondsUTCChanged);
			if(propertyNameVisible)
				this._propertyBindingVisible= PropertyBindingFactory.standard(this, model, propertyNameVisible, this._visibleChanged);
		}
		
		get element(){
			return this._element;
		}
		_millisecondsUTCChanged(value){
			this._updateYears(value);
		}
		_updateYears(millisecondsUTC){
			if(this._currentlySelectedEntry){
				this._currentlySelectedEntry.selected=false;
				this._currentlySelectedEntry=null;
			}
			if(isNullOrUndefined(millisecondsUTC))
			{
				this._currentYearToStartFrom = null;
				return;
			}
			const date = new Date(millisecondsUTC);
			const entryForYear = this._updateEntriesFromYear(this._getYearToStartFrom(date), date.getYear()+1900);
			if(entryForYear){
				this._currentlySelectedEntry = entryForYear;
				this._currentlySelectedEntry.selected=true;
			}
		}
		_updateEntriesFromYear(yearToStartFrom, selectedYear){
			var entryForSelectedYear = null;
			for(var i=0; i<this._entries.length; i++){
				const entry = this._entries[i];
				const year = yearToStartFrom++;
				entry.value = year;
				if(year===selectedYear)
					entryForSelectedYear= entry;
			}
			return entryForSelectedYear;
		}
		_getYearToStartFrom(date){
			if(!isNullOrUndefined(this._currentYearToStartFrom))
			return this._currentYearToStartFrom;
			const year = date.getYear()+1900;
			const startOfDecade = N_YEARS<10?year:year-(year%10);
			this._currentYearToStartFrom = startOfDecade;
			return this._currentYearToStartFrom;
		}
		_visibleChanged(value){
			if(value)
				this._element.classList.add('visible');
			else
				this._element.classList.remove('visible');
		}
		_handleClickedLeft(){
			this._currentYearToStartFrom-=N_YEARS;
			this._updateYears(this._propertyBindingMillisecondsUTC.get());
		}
		_handleClickedRight(){
			this._currentYearToStartFrom+=N_YEARS;
			this._updateYears(this._propertyBindingMillisecondsUTC.get());
		}
		_handleClickedEntry(year){
			const millisecondsUTC = this._propertyBindingMillisecondsUTC.get();
			const date = isNullOrUndefined(millisecondsUTC)?new Date():new Date(millisecondsUTC);
			date.setYear(year);
			this._propertyBindingMillisecondsUTC.set(date.getTime());
		}
		dispose(){
			this._disposes.forEach(d=>d());
			this.myBindings.dispose();
		}
    }