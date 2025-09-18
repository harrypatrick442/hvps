export default class ClickedOffHandle{
	constructor(element, callbackHide, callbackDispose){
		this._additionalElements =[];
		this._disposed = false;
		this._callbackHide = callbackHide;
		this._element=element;
		this._callbackDispose = callbackDispose;
		this.addAdditionalElement = this.addAdditionalElement.bind(this);
		this.removeAdditionalElement = this.removeAdditionalElement.bind(this);
		this.getElements = this.getElements.bind(this);
		this.getElement = this.getElement.bind(this);
		this.hide = this.hide.bind(this);
		this.dispose = this.dispose.bind(this);
	}
	addAdditionalElement(additionalElement){
		if(this._additionalElements.indexOf(additionalElement)<0){
			this._additionalElements.push(additionalElement);
		}
	}
	removeAdditionalElement(element){
		var index = this._additionalElements.indexOf(element);
		if(index<0)return;
		this._additionalElements.splice(index, 1);
	}
	getElements(){
		var elements = [this._element];
		return elements.concat(this._additionalElements);
	}
	getElement(){
		return this._element;
	}
	getDisposed(){
		return this._disposed;
	}
	hide(){
		if(this._disposed)return;
		this._disposed = true;
		this._callbackDispose(this);
		this._callbackHide();
	}
	dispose(){
		if(this._disposed)return;
		this._disposed = true;
		this._callbackDispose(this);
	}
}