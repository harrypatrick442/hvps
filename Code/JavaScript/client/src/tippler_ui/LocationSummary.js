import E from '../ui_core/E';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import exposeMethod from '../mvvm/exposeMethod';
import exposeBinding from '../mvvm/exposeBinding';
import eventEnable from '../core/eventEnable';
import isNullOrUndefined from '../core/isNullOrUndefined';
import EditableTextBlock from '../tippler_ui/EditableTextBlock';
import TextBlock from '../tippler_ui/TextBlock';
import LocationPickerMenuViewModel from '../view_models/location/LocationPickerMenuViewModel';
import './LocationSummary.css';
export default class LocationSummary{
	constructor({model, propertyNameAbstractLocation, editable}){
		eventEnable(this);
		this._abstractLocationChanged = this._abstractLocationChanged.bind(this);
		this._showLocationPicker = this._showLocationPicker.bind(this);
		this._save = this._save.bind(this);
		this.dispose = this.dispose.bind(this);
		exposeBinding(this, '_text', ()=>this._text);
		exposeMethod(this, '_showLocationPicker', this._showLocationPicker);
		this._propertyBindingAbstractLocation = PropertyBindingFactory.standard(
			this, model, propertyNameAbstractLocation, this._abstractLocationChanged);
		if(editable){
			this._textBlock = new EditableTextBlock({model:this, propertyNameText:'_text',
				methodNameEdit:'_showLocationPicker', className:'location-summary'});
		}
		else{
			this._textBlock = new TextBlock({
				className:'location-summary', propertyName:'_text', model:this});
		}
	}
	get _text(){
		const t = this._abstractLocation?.formattedAddress;
		return isNullOrUndefined(t)?'':t;
	}
	get element(){
		return this._textBlock.element;
	}
	_abstractLocationChanged(value){
		this._abstractLocation = value;
		this.bindingsHandler.changed('_text', this._text);
	}
	_showLocationPicker(){
		LocationPickerMenuViewModel.instance.show({save:this._save, abstractLocation:this._abstractLocation});
	}
	_save(abstractLocation){
		this._propertyBindingAbstractLocation.set(abstractLocation);
		this.dispatchEvent({type:'changed'});
	}
	dispose(){
		this._textBlock?.dispose();
		this.myBindings.dispose();
		this.bindingsHandler.dispose();
	}
};