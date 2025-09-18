import E from '../ui_core/E';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import TextBox from '../tippler_ui/TextBox';
import i from '../ui_core/i';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import _addElseRemoveClass from '../ui_core/_addElseRemoveClass';
import ClickedOffManager from '../ui_core/ClickedOffManager';
import './MapTextSearch.scss';
export default class MapTextSearch{
	constructor({model, propertyNameFormattedAddress, propertyNameAddressComponents,
			propertyNameExpanded, propertyNameLatLng, propertyNamePlaceName
		}){
		this._model = model;
		const disposes = (this._disposes = []);
		this._expandedChanged = this._expandedChanged.bind(this);
		this._fillInAddress= this._fillInAddress.bind(this);
		this._textBox = new TextBox({model, propertyName:propertyNameFormattedAddress,
			classNames:['text-search', 'autocomplete'],
			placeholder:'Search place....'});
		this._element = E.div('map-text-search');
		this._element.appendChild(this._textBox.element);
		const searchButton = _createImageHoverButton(i('Search'), i('SearchHover'), 'search-button',
			()=>this._bindingExpanded.set(true), this._disposes);
		this._element.appendChild(searchButton);
		model.autocomplete.setup({
			element:this._textBox.element, 
			callback:this._fillInAddress
		});
		
		//this._autocomplete = new window.google.maps.places.Autocomplete(this._element, {types: ['geocode']});
		//this._autocomplete.addListener('place_changed', this._fillInAddress);
		this._propertyBindingLatLng = PropertyBindingFactory.standard(this, model, propertyNameLatLng, this._latLngChanged);
		this._propertyBindingAddressComponents = PropertyBindingFactory.standard(this, model, propertyNameAddressComponents, ()=>{});
		if(propertyNamePlaceName)
			this._bindingPlaceName = PropertyBindingFactory.standard(this, model, propertyNamePlaceName, ()=>{});
		this._bindingExpanded = PropertyBindingFactory.standard(this, model, propertyNameExpanded, this._expandedChanged);		
		this._clickedOffManagerTextBox = new ClickedOffManager({
			element:this._textBox.element, hide:()=>this._bindingExpanded.set(false)});
	}
	get element(){
		return this._element;
	}
	_fillInAddress({latLng, addressComponents, name}) {
		//var place = this._autocomplete.getPlace();
		//const location = place.geometry.location;
		this._propertyBindingLatLng.set(latLng);
		this._propertyBindingAddressComponents.set(addressComponents);
		this._bindingPlaceName?.set(name);
	}
	_expandedChanged(value){
		this._clickedOffTextBoxHandle?.dispose();
		if(value){
			this._clickedOffTextBoxHandle = this._clickedOffManagerTextBox.register();
			this._textBox.element.focus();
		}
		_addElseRemoveClass(value, 'expanded', this._element);
	}
	_latLngChanged(){
		
	}
	dispose(){
		this._disposes.forEach(d=>d());
		this.myBindings?.dispose();
	}
}