import eventEnable from '../core/eventEnable';
import LoopBreak from '../core/LoopBreak';
import PropertyHelper from '../mvvm/PropertyHelper';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import ValidityIndicator from './ValidityIndicator';
import getConvertToType from '../core/getConvertToType';
let count =0;
export default class Select{
    constructor (props) {
        const {className, classNames, readOnly, itemTextName, itemValueName, 
			propertyNameItemText, propertyNameItemValue, propertyNameItems, propertyNameSelectedItem, propertyNameSelectedValue, 
			propertyNameReadOnly, valueConverter,model,addNoneOption,
			validate}=props;
		let {converterType}=props;
        eventEnable(this);
		this.nSelect = count++;
		this.focus=this.focus.bind(this);
		this.dispose=this.dispose.bind(this);
		this._itemsChanged=this._itemsChanged.bind(this);
		this._updateItems=this._updateItems.bind(this);
		this._selectedItemChanged=this._selectedItemChanged.bind(this);
		this._selectedValueChanged=this._selectedValueChanged.bind(this);
		this.clearOptions=this.clearOptions.bind(this);
		this.setSelectedItem=this.setSelectedItem.bind(this);
		this.setSelectedValue=this.setSelectedValue.bind(this);
		this._changed=this._changed.bind(this);
		this._updateToModel=this._updateToModel.bind(this);
		this._addEventListener=this._addEventListener.bind(this);
		this._readOnlyChanged=this._readOnlyChanged.bind(this);
		this._setReadOnlyClasses=this._setReadOnlyClasses.bind(this);

		if(propertyNameItemText)
			this._propertyNameReadOnly=PropertyHelper.getGetterName(propertyNameItemText);
		if(propertyNameItemValue)
			this._getterNameItemValue= PropertyHelper.getGetterName(propertyNameItemValue);
		this._valueConverter = valueConverter;
		this._model = model;
		if(!this._model)throw new Error('No this._model provided. propertyNameItems is '+propertyNameItems);
        this._selectedItem = null;
        this._selectedValue = null;
		this._addNoneOption = addNoneOption;
		this._itemTextName=itemTextName;
		this._propertyNameSelectedItem = propertyNameSelectedItem;
		this._propertyNameSelectedValue = propertyNameSelectedValue;
		this._itemValueName = itemValueName;
		this._readOnly = readOnly;
		this._loopBreakDefault=new LoopBreak();
		this._element = document.createElement('div');
        this._element.classList.add('select');
        this._select = document.createElement('select');
		this._readOnlyElement = document.createElement('div');
		this._readOnlyElement.classList.add('readonly0');
        this._mapValueToItems = {};
		this._getValueFromItem = (item)=>this._itemValueName ? item[this._itemValueName] : (this._getterNameItemValue? item[this._getterNameItemValue]():item);;
        if (this._readOnly)
            this._select.readOnly = true;
        if (className)
            this._element.classList.add(className);
        if (classNames) classNames.forEach((className)=>{
            this._element.classList.add(className);
        });
        this._select.addEventListener('change', this._changed);
       
		if(validate){
			this._validityIndicator = new ValidityIndicator(this, this._model,
			this._propertyNameSelectedItem?this._propertyNameSelectedItem:this._propertyNameSelectedValue, this._element);
		}
		if(propertyNameReadOnly){
			this._propertyBindingReadOnly = PropertyBindingFactory.standard(this, this._model, propertyNameReadOnly, this._readOnlyChanged, true);
			this._readOnly = this._propertyBindingReadOnly.get();
		}
		if(this._readOnly){
			this._element.appendChild(this._readOnlyElement);
		}
		else{
			this._element.appendChild(this._select);
		}
		this._setReadOnlyClasses(this._readOnly);
		this._propertyBindingItems = PropertyBindingFactory.standard(this, this._model, propertyNameItems, this._itemsChanged, true);
		var items = this._propertyBindingItems.get();
		if(!items)throw new Error('Items is: '+items+' for propertyNameItems '+propertyNameItems+(this._model?(' on this._model of type '+this._model.constructor.name):'')); 
        this._updateItems(items);
        if (this._propertyNameSelectedItem) {
            this._propertyBindingSelectedItem = PropertyBindingFactory.standard(this, this._model, this._propertyNameSelectedItem,
			this._selectedItemChanged, true);
            this.setSelectedItem(this._propertyBindingSelectedItem.get());
        }
        else if (this._propertyNameSelectedValue) {
            this._propertyBindingSelectedValue = PropertyBindingFactory.standard(this, this._model, this._propertyNameSelectedValue,
			this._selectedValueChanged, true);
            this.setSelectedValue(this._propertyBindingSelectedValue.get());
			if(!this._valueConverter){
				if(!converterType)converterType=typeof(this._selectedValue);
				this._valueConverter = getConvertToType(converterType);
			}
        }
	} 
	focus() {
		this._select.focus();
	}
	dispose () {
		this.myBindings&&this.myBindings.dispose();
	}
	get element() { 
		return this._element; 
	}
	get validityIndicator(){
		return this._validityIndicator;
	}
	_itemsChanged(value) {
		this._updateItems(value);
	}
	_updateItems(items) {
		this.clearOptions();
		this._mapValueToItems = {};
		var containedSelected = false;
		if(!items){
			console.error(this._propertyBindingItems+' has no get method on object of type'+ this._model.constructor.name);
		}
		if(this._addNoneOption){
			const option = document.createElement('option');
			option.text = '';
			option.value = undefined;
			this._select.appendChild(option);
		}
		items.forEach((item)=>{
			var value = this._getValueFromItem(item);// todo can make this more efficient when get time.
			if ((this._propertyNameSelectedItem && item == this._selectedItem) 
				|| (this._propertyNameSelectedValue && value === this._selectedValue)) {
					containedSelected = true;
			}
			const text = this._itemTextName ? item[this._itemTextName] : (this._propertyNameReadOnly? item[this._propertyNameReadOnly](): item);
			const option = document.createElement('option');
			option.text = text;
			option.value = value;
			this._select.appendChild(option);
			this._mapValueToItems[value] = item;
		});
		if (containedSelected) {
			if (this._propertyNameSelectedItem) this.setSelectedItem(this._selectedItem);
			else this.setSelectedValue(this._selectedValue);
		}
	}
	_selectedItemChanged(value) {
		this.setSelectedItem(value, true);
		
	}
	_selectedValueChanged(value) {
		this.setSelectedValue(value, true);
	}
	clearOptions() {
		while (this._select.options.length > 0)
			this._select.removeChild(this._select.options[0]);
	}
	setSelectedItem(item, skipUpdateToModel) {
		if(!item){this.setSelectedValue();return;}
		const value = this._itemValueName?item[this._itemValueName]:(this._getterNameItemValue?item[this._getterNameItemValue]():item);
		this._selectedItem = value;
		this.setSelectedValue(value, skipUpdateToModel);
	}
	setSelectedValue(value, skipUpdateToModel) {
		this._selectedValue = value;
		value = String(value);
		for (var i = 0; i < this._select.options.length; i++) {
			const option = this._select.options[i];
			if (option.value === value) {
				this._select.selectedIndex = i;
				this._readOnlyElement.innerHTML = option.text;
				return;
			}
		}
		/*not matched any items*/
		if(this._select.options.length<1||this._addNoneOption)return;
		this._select.selectedIndex=0;
		if(this._select.options[0])
			this._readOnlyElement.innerHTML=this._select.options[0].text;
		//defaulting has the potential to trigger a runaway condition if the default is not there. Hence the loop break to make sure that cant happen.
		if(this._loopBreakDefault.trigger())return;
		if(skipUpdateToModel)return;
		this._updateToModel(this._select.options[0]);
	}
	_changed() {
		var selectedOption = this._select.options[this._select.selectedIndex];
		if (!selectedOption) return;
		this._updateToModel(selectedOption);
	}
	_updateToModel(selectedOption){
		const item = this._mapValueToItems[selectedOption.value];
		if (this._propertyNameSelectedItem) {
			this._selectedItem = item;
			this._propertyBindingSelectedItem.set(this._selectedItem);
		}
		else {
			const value = this._getValueFromItem(item);
			this._selectedValue = this._valueConverter?this._valueConverter(value):value;
			this._propertyBindingSelectedValue.set(this._selectedValue);
		}
	}
	_addEventListener(name, callback) {
		switch (name) {
			case 'change':
				this._select.addEventListener(name, callback);
				break;
		}
	}
	_readOnlyChanged(value){
		if(this._readOnly==value)return;
		if(value){
			this._element.removeChild(this._select);
			this._element.appendChild(this._readOnlyElement);
			return;
		}
		this._element.appendChild(this._select);
		this._element.removeChild(this._readOnlyElement);
		this._setReadOnlyClasses(value);
	}
	_setReadOnlyClasses(readOnly){
		if(readOnly)
			this._element.classList.add('readonly');
		else
			this._element.classList.remove('readonly');
	}
}