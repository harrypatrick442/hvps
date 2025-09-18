import ParameterErrors from '../errors/ParameterErrors';
import InsertedMenuItemsRegistrationHandle from './InsertedMenuItemsRegistrationHandle';
const { throwNotProvided, throwNotOfType} = ParameterErrors;
export default class InsertedMenuItemsHandler{
	constructor(getInsertedMenuItems, setInservedMenuItems){
		if(!setInservedMenuItems)
			throwNotProvided('setInservedMenuItems');
		if(!getInsertedMenuItems)
			throwNotProvided('getInsertedMenuItems');
		this._setInservedMenuItems = setInservedMenuItems;
		this._getInsertedMenuItems = getInsertedMenuItems;
		this._getCallbackRemoveLexicalClosure = this._getCallbackRemoveLexicalClosure.bind(this);
		this.registerMenuItems = this.registerMenuItems.bind(this);
	}
	registerMenuItems(menuItems){
		const insertedMenuItems = this._getInsertedMenuItems();
		menuItems.forEach(menuItem=>insertedMenuItems.push(menuItem));
		this._setInservedMenuItems(insertedMenuItems);
		const callbackRemove = this._getCallbackRemoveLexicalClosure(menuItems);
		return new InsertedMenuItemsRegistrationHandle({callbackRemove:callbackRemove});
	}
	_getCallbackRemoveLexicalClosure(menuItems){
		return ()=>{
			const insertedMenuItems =  this._getInsertedMenuItems();
			menuItems.forEach(menuItem=>{
				const index = insertedMenuItems.indexOf(menuItem);
				if(index<0)return;
				insertedMenuItems.splice(index, 1);
			});
			this._setInservedMenuItems(insertedMenuItems);
		};
	}
}