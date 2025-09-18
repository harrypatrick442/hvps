import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import OrderedItems from './OrderedItems';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import TabPanel_Tab from './TabPanel_Tab';
import TabPanel_Panel from './TabPanel_Panel';
import './TabPanel.css';
const { throwNotProvided } = ParameterErrors;
export default class TabPanel {
    constructor({model, propertyNameEntries, propertyNameText, propertyNameActiveTab, createChildView,
		onlyOneTabAtOnce, callbackCreateTabInnerContent}) {
		this._callbackCreateTabInnerContent = callbackCreateTabInnerContent;
		this._onlyOneTabAtOnce = isNullOrUndefined(onlyOneTabAtOnce)?true:onlyOneTabAtOnce;
		eventEnable(this);
		this._propertyNameText = propertyNameText;
		this._propertyNameActiveTab= propertyNameActiveTab;
		this._createChildView = createChildView;
		this._createTab= this._createTab.bind(this);
		this._createPanel= this._createPanel.bind(this);
		this._handleTabClick= this._handleTabClick.bind(this);
		this._handleTabDispose= this._handleTabDispose.bind(this);
		this._handlePanelDispose= this._handlePanelDispose.bind(this);
		this._setActiveTab= this._setActiveTab.bind(this);
		this._updateMultiple= this._updateMultiple.bind(this);
		this._element = document.createElement('div');
		this._element.classList.add('tab-panel');
		this._tabsElement = document.createElement('tabs');
		this._tabsElement.classList.add('tabs');
		this._panelsElement = document.createElement('panels');
		this._panelsElement.classList.add('panels');
		this._element.appendChild(this._tabsElement);
		this._element.appendChild(this._panelsElement);
		this._tabs = [];
		this._orderedItemsTabs = new OrderedItems({element:this._tabsElement, model,  propertyNameItems:propertyNameEntries, createView:this._createTab});
	}
	_createTab(model){
		const panel = this._createPanel(model);
		const tab =  new TabPanel_Tab({model, propertyNameText:this._propertyNameText, 
		propertyNameActiveTab:this._propertyNameActiveTab, callbackDispose:this._handleTabDispose, associatedPanel:panel,
		becameActive:this._onlyOneTabAtOnce?this._setActiveTab:()=>{},
		callbackCreateInnerContent:this._callbackCreateTabInnerContent});
		this._tabs.push(tab);
		this._updateMultiple();
		tab.addEventListener('click', this._handleTabClick);
		this._setActiveTab(tab);
		return tab;
	}
	_updateMultiple(){
		if(this._tabs.length>1)
			this._element.classList.add('multiple');
		else 
			this._element.classList.remove('multiple');
	}
	_handleTabClick({tab}){
		this._setActiveTab(tab);
	}
	_setActiveTab(tab){
		for (var t of this._tabs)
		{
			if(t===tab){
				tab.setActive(true);
				continue;
			}
			t.setActive(false);
		}
	}
	_handleTabDispose(tab){
		tab.removeEventListener('click', this._handleTabClick);
		this._tabs.splice(this._tabs.indexOf(tab), 1);
		this._updateMultiple();
	}
	_handlePanelDispose(panel){
		this._panelsElement.removeChild(panel.element);
	}
	_createPanel(model){
		const panel = new TabPanel_Panel({model, createChildView:this._createChildView, callbackDispose:this._handlePanelDispose});
		this._panelsElement.appendChild(panel.element);
		return panel;
	}
	get element(){
		return this._element;
	}
	setActive(value){
		const tab = this._tabs.filter(tab=>tab.value===value)[0];
		if(!tab)throw new Error(`Value ${value} did not match any tab value`);
		if(this._currentTab){
			this._currentTab.tabElement.classList.remove('active');
			this._currentTab.panelElement.classList.remove('active');
		}
		tab.tabElement.classList.add('active');
		tab.panelElement.classList.add('active');
		this._currentTab = tab;
		this.dispatchEvent({type:'changed', value:tab.value});
	}
}