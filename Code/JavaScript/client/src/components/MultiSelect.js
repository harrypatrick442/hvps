import i from '../ui_core/i';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import isNullOrUndefined from '../core/isNullOrUndefined';
import eventEnable from '../core/eventEnable';
import ClickedOff from '../ui_core/ClickedOff';
import LoopBreak from '../core/LoopBreak';
import WhenFinishedModifying from '../core/WhenFinishedModifying';
import E from '../ui_core/E';
import getAbsoluteRectangle from '../ui_core/getAbsoluteRectangle';
import MultiSelectItem from './MultiSelectItem';
import PageKeys from '../core/PageKeys';
import './MultiSelect.css';
export default class MultiSelect{
    constructor ({className}) {
		const disposes =[];
		this._disposes = disposes;
		eventEnable(this);
		this._expand = this._expand.bind(this);
		this._collapse = this._collapse.bind(this);
		this._toggleExpand = this._toggleExpand.bind(this);
		this._itemSelectedChanged = this._itemSelectedChanged.bind(this);
		this._updateText = this._updateText.bind(this);
		this._clickedOff = this._clickedOff.bind(this);
		this._setExpanded = this._setExpanded.bind(this);
		this._constrainItems = this._constrainItems.bind(this);
		this._handleItemskeys = this._handleItemskeys.bind(this);
		this.dispose = this.dispose.bind(this);
		this._loopBreak = new LoopBreak();
		this._element = E.div('multi-select');
		this._textElement = E.div('text'); 
		this._textInnerElement = E.div('inner');
		this._textElement.appendChild(this._textInnerElement);
		this._buttonExpand = _createImageHoverButton(i('DownArrow'), i('DownArrowHover'), 'expand-button', this._expand, disposes);
		this._buttonCollapse = _createImageHoverButton(i('UpArrow'), i('UpArrowHover'), 'collapse-button', this._collapse, disposes);
		this._textElement.addEventListener('mousedown', this._toggleExpand);
		this._element.appendChild(this._textElement);
		this._element.appendChild(this._buttonCollapse);
		this._element.appendChild(this._buttonExpand);
		this._itemsElement = E.div('items');
		this._element.appendChild(this._itemsElement);
		if(className)
			this._element.classList.add(className);
		const entered = (e)=>{
			const {target}=e;
			console.log(target);
			setTimeout(()=>{
				this._element.classList.add('hovering');
			}, 0);//just incase to make sure enter fires after leave on another sub component
		};
		const left = (e)=>{
			const {target}=e;
			console.log(target);
			this._element.classList.remove('hovering');
		};
		[this._buttonExpand, this._buttonCollapse].forEach(e=>{			
			e.addEventListener('focus', entered);
			e.addEventListener('blur', left);
		});
		[this._element].forEach(e=>{
			e.addEventListener('mouseenter', entered);
			e.addEventListener('mouseleave', left);
		});
	}
	get element(){
		return this._element;
	}
	get itemsElement(){
		return this._itemsElement;
	}
	get selectedValues(){
		if(isNullOrUndefined(this._entries))
			return [];
		return this._entries.filter(entry=>entry.selected).map(entry=>entry.value);
	}
	set selectedValues(values){
		if(isNullOrUndefined(this._entries))return;
		this._loopBreak.trigger();
		if(isNullOrUndefined(values)||values.length<1){
			this._entries.forEach(entry=>entry.selected=false);
			return;
		}
		const valuesSet = new Set(values);
		this._entries.forEach(entry=>{
			entry.selected = valuesSet.has(entry.value);
		});
	}
	set items(items){
		this._entries?.forEach(entry=>{
			this._itemsElement.removeChild(entry.element);
			entry.dispose();
		});
		if(isNullOrUndefined(items)){
			this._entries = null;
			return;
		}
		this._entries = items.map(({text, value})=>{
			const multiSelectItem = new MultiSelectItem({text, value, selectedChanged:this._itemSelectedChanged,
				callbackSetFocused:(entry)=>this._focussedEntry = entry});
			this._itemsElement.appendChild(multiSelectItem.element);
			return multiSelectItem;
		});
	}
	_toggleExpand(){
		this._setExpanded(!this._expanded);
	}
	_expand(){
		this._setExpanded(true);
	}
	_collapse(){
		this._setExpanded(false);
	}
	_itemSelectedChanged(item, cameFromClick){
		this._updateText();
		if(cameFromClick){
			this._buttonExpand.focus();
		}
		if(this._loopBreak.trigger())return;
		this.dispatchEvent({type:'changed', multiSelect:this, value:item?item.value:null});
	}
	_updateText(){
		if(isNullOrUndefined(this._entries)){
			this._textInnerElement.innerHTML = '';
			return;
		}
		const strs = this._entries.filter(entry=>entry.selected).map(entry=>entry.text);
		strs.sort((a, b)=>a.localeCompare(b));
		const str = strs.join(', ');
		this._textInnerElement.innerHTML = str;
	}
	_setExpanded(value){
		this._expanded = value;
		if(this._clickedOffHandle)this._clickedOffHandle.dispose();
		if(this._disposeItemsKeyRegistration)
			this._disposeItemsKeyRegistration();
		if(value){
			this._clickedOffHandle = ClickedOff.register(this._itemsElement, this._clickedOff);
			this._disposeItemsKeyRegistration = PageKeys.instance.registerKeyDown(this._handleItemskeys);
			this._element.classList.add('expanded');
			this._constrainItems();
			return;
		}
		this._clickedOffHandle = null;
		this._element.classList.remove('expanded');
	}
	_constrainItems(){
		this._itemsElement.style.maxHeight = 'unset';
		const rect = getAbsoluteRectangle(this._element);
		const pageWidth = document.body.offsetWidth;
		const pageHeight = document.body.offsetHeight;
		const maxHeightBelow = pageHeight - (rect.t+rect.h);
		const maxHeightAbove = rect.t;
		if(maxHeightAbove > maxHeightBelow){
			this._element.classList.add('above');			
			this._element.classList.remove('below');			
			this._itemsElement.style.maxHeight = `${maxHeightAbove-3}px`;
		}
		else{
			this._element.classList.remove('above');			
			this._element.classList.add('below');			
			this._itemsElement.style.maxHeight = `${maxHeightBelow-3}px`;
			this._itemsElement.style.maxHeight = `${maxHeightBelow-10}px`;
		}	
	}
	_clickedOff(){
		this._setExpanded(false);
	}
	dispose(){
		this._textElement.removeEventListener('mousedown', this._toggleExpand);
		this._entries?.forEach(entry=>entry.dispose());
		this._disposes.forEach(d=>d());
	}
	_handleItemskeys({keyCode}){
		switch(keyCode){
			case 9:
				this._setExpanded(false);
				break;
			case 27:
				this._setExpanded(false);
				this._buttonExpand.focus();
				break;
			case 40:
			case 39:
				{	
					if(isNullOrUndefined(this._focussedEntry)){
						this._entries[0].focus();
						break;
					}
					let nextIndex = this._entries.indexOf(this._focussedEntry)+1;
					if(nextIndex>=this._entries.length)
						nextIndex=0;
					this._entries[nextIndex].focus();
				}
				break;
			case 38:
			case 37:
				{
					if(isNullOrUndefined(this._focussedEntry)){
						this._entries[this._entries.length-1].focus();
						break;
					}
					let nextIndex = this._entries.indexOf(this._focussedEntry)-1;
					if(nextIndex<0)
						nextIndex=this._entries.length-1;
					this._entries[nextIndex].focus();
				}
				break;
		}
	}
}