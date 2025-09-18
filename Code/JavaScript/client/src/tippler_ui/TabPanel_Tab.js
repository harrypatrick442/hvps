import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
import OrderedItems from './OrderedItems';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
const { throwNotProvided } = ParameterErrors;
export default class TabPanel_Tab {
    constructor({model, propertyNameText, callbackDispose, propertyNameActiveTab, associatedPanel, becameActive, callbackCreateInnerContent}) {
		eventEnable(this);
		this._disposes= [];
		if(propertyNameActiveTab===undefined)
			propertyNameActiveTab= 'activeTab';
		this._callbackDispose = callbackDispose;
		this._associatedPanel=associatedPanel;
		this._becameActive = becameActive;
		this._activeTabChanged = this._activeTabChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._dispatchClick= this._dispatchClick.bind(this);
		this.setActive= this.setActive.bind(this);
		this._element = document.createElement('div');
		this._element.classList.add('tab');
		const tabInnerElement = document.createElement('div');
		tabInnerElement.classList.add('inner');
		if(callbackCreateInnerContent){
				callbackCreateInnerContent({model, tab:this, tabInnerElement, disposes:this._disposes});
		}
		else
			this._propertyBindingText= PropertyBindingFactory.standard(this, model, propertyNameText, (value)=>tabInnerElement.textContent =value);
		this._propertyBindingActiveTab = PropertyBindingFactory.standard(this, model, propertyNameActiveTab, this._activeTabChanged);
		this._element.addEventListener('click', this._dispatchClick);
		this._element.appendChild(tabInnerElement);
	}
	_activeTabChanged(value){
		if(value)
			this._element.classList.add('active');
		else
			this._element.classList.remove('active');
		this._associatedPanel.setActive(value);
		if(value)
			this._becameActive(this);
	}
	get associatedPanel(){
		return this._associatedPanel;
	}
	setActive(value){
		this._propertyBindingActiveTab.set(value);
	}
	dispose(){
		this._disposes.forEach(d=>d());
		this._associatedPanel.dispose();
		this.myBindings.dispose();
		this._callbackDispose(this);
	}
	_dispatchClick(){
		this.dispatchEvent({type:'click', tab:this});
	}
	get element(){
		return this._element;
	}
}