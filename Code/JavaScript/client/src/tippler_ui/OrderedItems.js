import HashBuilder from '../core/HashBuilder';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import isDescendant from '../ui_core/isDescendant';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
export default class OrderedItems{
		constructor(props) {
			this._createdAt = new Error().stack;
			const {element, model, propertyNameItems, createView, convertItem, itemsArePrimitives}=props;
			eventEnable(this);
			if(!model)throw new Error('No model provided');
			if(!createView)throw new Error('createView not provided');
			this._createView = createView;
			this._element = element;
			this.getViewFromItem= this.getViewFromItem.bind(this);
			this.getViewFromItemHash= this.getViewFromItemHash.bind(this);
			this.getViewFromItemId= this.getViewFromItemId.bind(this);
			this._itemsChanged= this._itemsChanged.bind(this);
			this._addItemAtIndex= this._addItemAtIndex.bind(this);
			this._repositionItemIfNecessary= this._repositionItemIfNecessary.bind(this);
			this._removeItemsById= this._removeItemsById.bind(this);
			this._removeItemById= this._removeItemById.bind(this);
			this._dispatchChagned= this._dispatchChagned.bind(this);
			this._views=[];
			this._mapItemIdToView= new Map();
			this._items = null;
			this._getHash = itemsArePrimitives?function(item){return String(item);}:function(item){return HashBuilder(item);};
			if(convertItem)
				this._propertyBindingItems = 
				(!itemsArePrimitives
				?PropertyBindingFactory.arrayConversion
				:PropertyBindingFactory.arrayConversionFromPrimitives
				)(this, model, propertyNameItems, convertItem, undefined, this._itemsChanged).carriedOver;
			else
				this._propertyBindingItems = PropertyBindingFactory.standard(this, model, propertyNameItems, this._itemsChanged);
			var hasItemsProperty=this._propertyBindingItems.get()?true:false;
			if(!hasItemsProperty)return;//throw new Error('No '+propertyNameItems+' property on model of type '+model.constructor.name);
		}
		get hasItems(){
			return this._views.length>0;
		}
		getViewFromItem(item){
			return this.getViewFromItemHash(item.hash);
		}
		getViewFromItemHash(hash){
			return this.getViewFromItemId(hash);
		}
		getViewFromItemId(itemId){
			return this._mapItemIdToView.get(itemId);
		}
		get views(){	
			return this._views;
		}
		get itemIds(){
			var list = Array.from(this._mapItemIdToView.keys());
			return list;
		}
		get items(){
			return this._items;
		}
		set items(value){
			this._propertyBindingItems.set(value);
		}
		dispose(){
			this.myBindings&&this.myBindings.dispose();
		}
		_itemsChanged(value){
			if(isNullOrUndefined(value))value =[];
			this._items = value;
			const itemsAndIndexToAdd = [];
			const itemIds =[];
			var lastElementPutInPlace;
			for(var i=0, item; item=this._items[i]; i++){
				const itemId = this._getHash(item);
				itemIds.push(itemId);
				var view = this._mapItemIdToView.get(itemId);
				if(!view){
					this._addItemAtIndex(item, i);
				}
				else
					this._repositionItemIfNecessary(item, view, i);
			}
			const itemIdsToRemove = this._mapItemIdToView.keys().filter(i=>itemIds.indexOf(i)<0);
			this._removeItemsById(itemIdsToRemove);
			this._dispatchChagned();
		}
		_addItemAtIndex(item, index){
			const view = this._createView(item);
			const viewElement = view.element;
			const hash = this._getHash(item);
			this._mapItemIdToView.set(hash, view);
			var nextView = this._views[index];
			if(!nextView){
				this._views.push(view);
				this._element.appendChild(view.element);
				if(view.appended)
					view.appended();
				return;
			}
			this._views.splice(index, 0, view);
			var nextViewElement = nextView.element;
			this._element.insertBefore(viewElement, nextViewElement);
			if(view.appended)
				view.appended();
		}
		_repositionItemIfNecessary(item, view, index){
			var currentIndex = this._views.indexOf(view);
			if(currentIndex==index)return;
			this._views.splice(currentIndex, 1);
			this._views.splice(index, 0, view);
			var viewsElement = view.element;
			this._element.removeChild(viewsElement);
			var nextView = this._views[index+1];
			if(!nextView)
				this._element.appendChild(viewsElement);
			else
			{
				var nextViewElement = nextView.element;
				this._element.insertBefore(viewsElement, nextViewElement);
			}				
		}
		_removeItemsById(itemIds){
			itemIds.forEach((itemId)=>{
				this._removeItemById(itemId);
			});
		}
		_removeItemById(itemId){
			var view = this._mapItemIdToView.get(itemId);
			if(!view)return;
			if(!view.dispose){
				throw new Error(`No dispose method on view of type ${view.constructor.name}`);
			}
			view.dispose();
			var index = this._views.indexOf(view);
			this._views.splice(index, 1);
			this._mapItemIdToView.delete(itemId);
			var viewElement = view.element;
			if(isDescendant(this._element , viewElement))
				this._element.removeChild(viewElement);
		}
		_dispatchChagned(){
			this.dispatchEvent({type:'changed'});
		}
    }