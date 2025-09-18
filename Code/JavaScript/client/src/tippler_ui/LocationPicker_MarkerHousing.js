import E from '../ui_core/E';
import isNullOrUndefined from '../core/isNullOrUndefined';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
export default class LocationPicker_MarkerHousing{
	constructor({model, propertyNameLatLng}){
		this._latLngChanged = this._latLngChanged.bind(this);
        this._element = E.div('marker-housing');	
		this._propertyBindingLatLng = PropertyBindingFactory.standard(this, model, propertyNameLatLng, this._latLngChanged);
	}
	get element(){
		return this._element;
	}
	_latLngChanged(value){
		if(value)
			this._element.classList.remove('visible');
		else	
			this._element.classList.add('visible');
	}
}
	