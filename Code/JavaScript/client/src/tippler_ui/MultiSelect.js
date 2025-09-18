import eventEnable from '../core/eventEnable';
import LoopBreak from '../core/LoopBreak';
import PropertyHelper from '../mvvm/PropertyHelper';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import isNullOrUndefined from '../core/isNullOrUndefined';
import MultiSelectComponent from '../components/MultiSelect';
export default class MultiSelect{
    constructor ({model, className, itemTextName, itemValueName, 
			propertyNameItems, propertyNameSelectedValues, propertyNameSelectedItems, defaultAbove
		}) {
		eventEnable(this);
		this._itemTextName = itemTextName;
		this._itemValueName = itemValueName;
		this._itemsChanged = this._itemsChanged.bind(this);
		this._selectedItemsChanged = this._selectedItemsChanged.bind(this);
		this._selectedValuesChanged = this._selectedValuesChanged.bind(this);
		this._handleChanged = this._handleChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._multiSelectComponent = new MultiSelectComponent({className, defaultAbove});
		this._multiSelectComponent.addEventListener('changed', this._handleChanged);
		this._mapValueToItem = new Map();
		this._itemToText = isNullOrUndefined(this._itemTextName)?(item)=>item:(item)=>item[this._itemTextName];
		this._itemToValue = isNullOrUndefined(this._itemValueName)?(item)=>item:(item)=>item[this._itemValueName];
		this._propertyBindingItems = PropertyBindingFactory.standard(this, model, propertyNameItems, this._itemsChanged);
		if(propertyNameSelectedItems)
			this._propertyBindingSelectedItems = PropertyBindingFactory.standard(this, model, propertyNameSelectedItems, this._selectedItemsChanged);
		if(propertyNameSelectedValues)
			this._propertyBindingSelectedValues = PropertyBindingFactory.standard(this, model, propertyNameSelectedValues, this._selectedValuesChanged);
	}
	get element(){
		return this._multiSelectComponent.element;
	}
	get itemsElement(){
		return this._multiSelectComponent.itemsElement;
	}
	_itemsChanged(items){
		this._mapValueToItem.clear();
		this._multiSelectComponent.items = items?.map(item=>{
			const value = this._itemToValue(item);
			this._mapValueToItem.set(value, item);
			return {text:this._itemToText(item), value};
		});
	}
	_selectedItemsChanged(selectedItems){
		this._multiSelectComponent.selectedValues = selectedItems?.map(item=>item.value);
	}
	_selectedValuesChanged(selectedValues){
		this._multiSelectComponent.selectedValues = selectedValues;
	}
	_handleChanged(){
		let selectedValues = this._multiSelectComponent.selectedValues;
		selectedValues = selectedValues?selectedValues:null;
		if(this._propertyBindingSelectedValues){
			this._propertyBindingSelectedValues.set(selectedValues);
		}
		if(this._propertyBindingSelectedItems){
			const items = selectedValues.map(value=>this._mapValueToItem.get(value));
			this._propertyBindingSelectedItems.set(items);
		}
		this.dispatchEvent({type:'changed'});
	}
	dispose(){
		this._multiSelectComponent.dispose();
		this.myBindings?.dispose();
		this._multiSelectComponent.removeEventListener('changed', this._handleChanged);
	}
}