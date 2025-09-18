import E from '../ui_core/E';
import LocationPicker_MarkerHousing from './LocationPicker_MarkerHousing';
import LocationPicker_Marker from './LocationPicker_Marker';
import Spinner from './Spinner';
import isNullOrUndefined from '../core/isNullOrUndefined';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import MapBase from './MapBase';
import './LocationPicker.css';
export default class LocationPicker extends MapBase{
	constructor({model, propertyNameLatLng, propertyNameBusy, 
		propertyNameFormattedAddress, propertyNameAddressComponents, 
		propertyNameTextSearchExpanded, propertyNamePlaceName}){
		super({model, propertyNameLatLng, propertyNameBusy,
			propertyNameFormattedAddress, propertyNameAddressComponents, 
			propertyNameTextSearchExpanded, propertyNamePlaceName});
		this.dispose = this.dispose.bind(this);
		this._moveEnd = this._moveEnd.bind(this);
		this._dragStarted= this._dragStarted.bind(this);
		this._element.classList.add('location-picker');
		this._markerHousing = new LocationPicker_MarkerHousing({model, propertyNameLatLng});
		this._marker = new LocationPicker_Marker({
			loopBreakMovingMapByLatLngChanged:this._loopBreakMovingMapByLatLngChanged,
			model, locationPickerElement:this._element, 
			mapElement:this._mapElement, getMap:()=>this._map,
			propertyNameLatLng, propertyNameFormattedAddress, 
			propertyNameAddressComponents
		});
		
		[this._markerHousing.element, this._marker.element]
			.forEach(e=>this._element.appendChild(e));
        //this._map.getView().on('change:resolution', ()=>this._zoomed());
        this._setMapZoom(16);
		
		const latLng = this._propertyBindingLatLng.get();
		if(!isNullOrUndefined(latLng)){
			this._loopBreakMovingMapByLatLngChanged.trigger();
			this._setMapCenter(latLng);
		}
		this._map.on('pointerdrag', ()=>this._dragStarted());
		this._map.on('moveend', ()=>this._moveEnd());
		this._disposes.push(model.addEventListener('moveMapToLatLng', ({latLng})=>{
			this._setMapCenter(latLng);
		}));
		//this._map.on('movestart', ()=>this._moveStart());
	}
	_dragStarted(){
		this._moving = true;
		this._propertyBindingLatLng.set(null);
		this._propertyBindingAddressComponents.set(null);
		this._propertyBindingFormattedAddress.set(null);
	}
	_moveEnd(){
		if(!this._moving)return;
		this._moving = false;
		if(this._loopBreakMovingMapByLatLngChanged.triggered)return;
		this._propertyBindingLatLng.set(null);
		this._propertyBindingAddressComponents.set(null);
		this._propertyBindingFormattedAddress.set(null);
	}
	dispose(){
		this._marker.dispose();
		this._markerHousing.dispose();
		super.dispose();
	}
};