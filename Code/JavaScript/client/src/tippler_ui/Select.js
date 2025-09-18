import eventEnable from '../core/eventEnable';
import LoopBreak from '../core/LoopBreak';
import PropertyHelper from '../mvvm/PropertyHelper';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import isNullOrUndefined from '../core/isNullOrUndefined';
import SelectComponent from '../components/Select';
export default class Select{
    constructor (props) {
        const {model, className, propertyNameSelectedValue, 
		propertyNameSelectedItem,  propertyNameItems, items,
				itemTextName, itemValueName, defaultAbove}=props;
        eventEnable(this);
		this._itemTextName = itemTextName;
		this._itemValueName = itemValueName;
		this._itemsChanged = this._itemsChanged.bind(this);
		this._selectedItemChanged = this._selectedItemChanged.bind(this);
		this._selectedValueChanged = this._selectedValueChanged.bind(this);
		this._handleChanged = this._handleChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._selectComponent = new SelectComponent({className, defaultAbove});
		this._selectComponent.addEventListener('changed', this._handleChanged);
		this._mapValueToItem = new Map();
		this._itemToText = isNullOrUndefined(this._itemTextName)?(item)=>item:(item)=>item[this._itemTextName];
		this._itemToValue = isNullOrUndefined(this._itemValueName)?(item)=>item:(item)=>item[this._itemValueName];
		if(propertyNameItems)
			this._propertyBindingItems = PropertyBindingFactory.standard(this, model, propertyNameItems, this._itemsChanged);
		else{
			this._selectComponent.items = items;
		}
		if(propertyNameSelectedItem)
			this._propertyBindingSelectedItem = PropertyBindingFactory.standard(this, model, propertyNameSelectedItem, this._selectedItemChanged);
		if(propertyNameSelectedValue)
			this._propertyBindingSelectedValue = PropertyBindingFactory.standard(this, model, propertyNameSelectedValue, this._selectedValueChanged);
		

	} 
	dispose () {
		this.myBindings&&this.myBindings.dispose();
	}
	get element() { 
		return this._selectComponent.element; 
	}
	get itemsElement(){
		return this._selectComponent.itemsElement;
	}
	_itemsChanged(items){
		this._mapValueToItem.clear();
		this._selectComponent.items = items?.map(item=>{
			const value = this._itemToValue(item);
			this._mapValueToItem.set(value, item);
			return {text:this._itemToText(item), value};
		});
	}
	
	_selectedItemChanged(selectedItem){
		this._selectComponent.selectedValue = this._itemToValue(selectedItem);
	}
	_selectedValueChanged(selectedValue){
		this._selectComponent.selectedValue = selectedValue;
	}
	_handleChanged(){
		let selectedValue = this._selectComponent.selectedValue;
		if(selectedValue===undefined)selectedValue = null;
		if(this._propertyBindingSelectedValue){
			this._propertyBindingSelectedValue.set(selectedValue);
		}
		if(this._propertyBindingSelectedItem){
			const item = this._mapValueToItem.get(selectedValue);
			this._propertyBindingSelectedItem.set(item);
		}
		this.dispatchEvent({type:'changed'});
	}
	dispose(){
		this._selectComponent.dispose();
		this.myBindings?.dispose();
		this._selectComponent.removeEventListener('changed', this._handleChanged);
	}
}