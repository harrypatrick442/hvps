import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import getAbsolute from '../ui_core/getAbsolute';
import OrderedItems from './OrderedItems';
export default class DragableOrderedItems{
		constructor(props) {
			let {element, model, propertyNameItems, createView, convertItem, propertyNameBeingHovered, propertyNameBeingMoved}=props;
			if(propertyNameBeingHovered===undefined)propertyNameBeingHovered='beingHovered';
			if(propertyNameBeingMoved===undefined)propertyNameBeingMoved='beingMoved';
			this._propertyNameBeingHovered = propertyNameBeingHovered;
			this._propertyNameBeingMoved = propertyNameBeingMoved;
			this._mapItemHashToItemInfo = new Map();
			this._setItemInfoCurrentlyHoveringOver = this._setItemInfoCurrentlyHoveringOver.bind(this);
			this._onStartMoveEntry = this._onStartMoveEntry.bind(this);
			this._onMoveEntry = this._onMoveEntry.bind(this);
			this._onEndMoveEntry = this._onEndMoveEntry.bind(this);
			this._reinsertItem = this._reinsertItem.bind(this);
			this._getMovingDownElseUp = this._getMovingDownElseUp.bind(this);
			this._itemsChanged = this._itemsChanged.bind(this);
			this._orderedItems = new OrderedItems({element, model, propertyNameItems, createView});
			this._propertyBindingItems = PropertyBindingFactory.standard(this, model, propertyNameItems, this._itemsChanged);
		}
		_itemsChanged(items){
			const seenItems = [];
			const mapItemHashToItemInfo = this._mapItemHashToItemInfo;
			items.forEach(item=>{
				const itemHash = HashBuilder(item);
				if(!mapItemHashToItemInfo.has(itemHash)){
					mapItemHashToItemInfo.set(itemHash, this._createItemInfo(item));
				}
				seenItems.push(item);
			});
			Array.from(mapItemHashToItemInfo.values()).forEach(mappedItem=>{
				if(!seenItems.indexOf(mappedItem)<=0)
					mapItemHashToItemInfo.delete(mappedItem.hash);
			});
		}
		get items(){
			return this._orderedItems.items;
		}
		set items(value){
			this._orderedItems.items=value;
		}
		_createItemInfo(item){
			const view = this._orderedItems.getViewFromItem(item);
			let itemInfo;
			const efficientMoveCycle = new EfficientMoveCycle({ element:view.element, stopPropagation:true, preventDefault:true,
				onStart:(e)=>this._onStartMoveEntry(e, itemInfo), onMove:(e)=>this._onMoveEntry(e, itemInfo), onEnd:(e)=>this._onEndMoveEntry(e, itemInfo), friendlyXY:true});
			const propertyBindingBeingHovered = PropertyBindingFactory.standard(this, item, this._propertyNameBeingHovered, ()=>{});
			const propertyBindingBeingMoved = PropertyBindingFactory.standard(this, item, this._propertyNameBeingMoved, ()=>{});
			itemInfo =  {efficientMoveCycle, item, view, propertyBindingBeingHovered, propertyBindingBeingMoved};
			return itemInfo;
		}
		_reinsertItem(items, item, index){
			if(index>=this.items.length||index===undefined||index===null){
				items.push(item);
			}
			else{
				items.splice(index, 0, item);
			}
		}
		_onStartMoveEntry(e, itemInfo){
			this._itemInfoBeingMoved = itemInfo;
			this._itemInfoBeingMoved.propertyBindingBeingMoved.set(true);
			this._firstEntryY = getAbsolute(itemInfo.view.element).top
			this._offsetMoveY =  (-15)-e.y;
		}
		_onMoveEntry(e, itemInfoThisEntry){
			const newY = e.y + this._offsetMoveY;
			let index=0;
			let itemInfoForEntryOver;
			const items = this.items;
			while(index<items.length){
				const item = items[index];
				itemInfoForEntryOver = this._mapItemHashToItemInfo.get(item.hash);
				const bottomOfElementAbsoluteY = getAbsolute(itemInfoForEntryOver.view.element).top;
				if(bottomOfElementAbsoluteY-newY>this._firstEntryY)
					break;
				index++;
			}
			if(itemInfoForEntryOver===null)
				itemInfoForEntryOver = this._mapItemHashToItemInfo.get(items[0].hash);
			this._setItemInfoCurrentlyHoveringOver(itemInfoForEntryOver===this._itemInfoBeingMoved?null:itemInfoForEntryOver);
		}
		_onEndMoveEntry(e, itemInfo){
			this._itemInfoBeingMoved.propertyBindingBeingMoved.set(false);
			if(!this._currentItemInfoHoveringOver){
				this._setItemInfoCurrentlyHoveringOver(null);
				return;
			}
			const movingDownElseUp = this._getMovingDownElseUp();
			if(this._itemInfoBeingMoved===this._currentItemInfoHoveringOver)return;
			const items = this.items;
			items.splice(items.indexOf(this._itemInfoBeingMoved.item), 1);
			const indexOfCurrentItemInfoHoveringOver = items.indexOf(this._currentItemInfoHoveringOver.item);
			const indexOfItemInfoBeingMoved = items.indexOf(this._itemInfoBeingMoved.item);
			this._reinsertItem(items, this._itemInfoBeingMoved.item, movingDownElseUp?indexOfCurrentItemInfoHoveringOver:(indexOfCurrentItemInfoHoveringOver+1));
			this.items = items;
			this._setItemInfoCurrentlyHoveringOver(null);
		}
		_setItemInfoCurrentlyHoveringOver(itemInfo){
			if(this._currentItemInfoHoveringOver)
				this._currentItemInfoHoveringOver.propertyBindingBeingHovered.set(false);
			this._currentItemInfoHoveringOver = itemInfo;
			if(itemInfo)this._currentItemInfoHoveringOver.propertyBindingBeingHovered.set(true);
		}
		_getMovingDownElseUp(){
			const items = this.items;
			const indexOfCurrentItemInfoHoveringOver = items.indexOf(this._currentItemInfoHoveringOver.item);
			const indexOfItemInfoBeingMoved = items.indexOf(this._itemInfoBeingMoved.item);
			return indexOfItemInfoBeingMoved>indexOfCurrentItemInfoHoveringOver;
		}
    }