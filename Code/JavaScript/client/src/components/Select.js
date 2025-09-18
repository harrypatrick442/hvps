import MultiSelect from './MultiSelect';
import isNullOrUndefined from '../core/isNullOrUndefined';
import LoopBreak from '../core/LoopBreak';
import eventEnable from '../core/eventEnable';
export default class Select{
    constructor ({className, defaultAbove}) {
		eventEnable(this);
		this.dispose = this.dispose.bind(this);
		this._handleChanged = this._handleChanged.bind(this);
		this._multiSelect = new MultiSelect({className, defaultAbove});
		this._element = this._multiSelect.element;
		this._element.classList.add('select');
		this._loopBreakChanged = new LoopBreak();
		this._multiSelect.addEventListener('changed', this._handleChanged);
	}
	get element(){
		return this._element;
	}
	get itemsElement(){
		return this._multiSelect.itemsElement;
	}
	get selectedValue(){
		const value = this._multiSelect.selectedValues[0];
		return value===undefined?null:value;
	}
	set selectedValue(value){
		this._multiSelect.selectedValues = isNullOrUndefined(value)?[]:[value];
	}
	set items(items){
		this._multiSelect.items = items;
		if(isNullOrUndefined(items)||items.length<1)return;
		if(isNullOrUndefined(this._currentlySelectedValue)){
			const value =  items[0].value;
			this._multiSelect.selectedValues = [value];
			this._currentlySelectedValue = value;
		}
	}
	dispose(){
		this._multiSelect.removeEventListener('changed', this._handleChanged);
		this._multiSelect.dispose();
	}
	_handleChanged({value}){
		if(this._loopBreakChanged.trigger())
			return;
		if(!isNullOrUndefined(value)){
			this._multiSelect.selectedValues = [value];
			this._currentlySelectedValue = value;
		}
		else{
			this._multiSelect.selectedValues = [this._currentlySelectedValue];
		}
		this.dispatchEvent({type:'changed'});
		this._multiSelect._setExpanded(false);
	}
}