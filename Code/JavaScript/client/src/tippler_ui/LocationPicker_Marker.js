import E from '../ui_core/E';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import isNullOrUndefined from '../core/isNullOrUndefined';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import GoogleGeocodingHelper from '../google/GoogleGeocodingHelper';
import _dynamicIcon from '../ui_core/_dynamicIcon';
import {fromLonLat, toLonLat, transformExtent} from 'ol/proj';
export default class LocationPicker_Marker{
	constructor({locationPickerElement, mapElement, getMap, model, propertyNameLatLng,
	propertyNameFormattedAddress, propertyNameAddressComponents,
	loopBreakMovingMapByLatLngChanged}){
        //eventEnable(this);
		this._model = model;
		this._loopBreakMovingMapByLatLngChanged = loopBreakMovingMapByLatLngChanged;
		const disposes = [];
		this._disposes = disposes;
		this._getMap = getMap;
		this._mapElement = mapElement;
		this._locationPickerElement = locationPickerElement;
		this._onStartMove = this._onStartMove.bind(this);
		this._onMove = this._onMove.bind(this);
		this._onEndMove = this._onEndMove.bind(this);
		this._setPosition = this._setPosition.bind(this);
		this._pixelOffsetToLatLng = this._pixelOffsetToLatLng.bind(this);
		this._latLngChanged = this._latLngChanged.bind(this);
		
        this._element = E.div('marker');
        this._imgElement = _dynamicIcon('ignores', 'DragDropLocationMarker', disposes);
        this._element.appendChild(this._imgElement);
        this._efficientMoveCycle = new EfficientMoveCycle({
			element:this._element,
			preventDefault:true,
			onStart:this._onStartMove, 
			onMove:this._onMove, 
			onEnd:this._onEndMove,
			friendlyXY:true
		});
		this._propertyBindingLatLng = PropertyBindingFactory.standard(this, model, propertyNameLatLng, this._latLngChanged);
		this._propertyBindingFormattedAddress = PropertyBindingFactory.standard(this, model, propertyNameFormattedAddress, ()=>{});
		this._propertyBindingAddressComponents = PropertyBindingFactory.standard(this, model, propertyNameAddressComponents, ()=>{});
	}
	get element(){
		return this._element;
	}
	_onStartMove({x, y}) {
		this._propertyBindingAddressComponents.set(null);
		this._propertyBindingFormattedAddress.set(null);
		const element = this.element;
		
		this._startOffsets = [element.offsetLeft - x, element.offsetTop - y];
		const markerWidth = element.offsetWidth,
			halfWidth = markerWidth / 2,
			markerHeight = element.offsetHeight;
		const mapElement = this._mapElement;
		this._boundaries = {xFrom:-halfWidth, yFrom:-markerHeight, xTo:mapElement.offsetWidth - halfWidth, yTo:mapElement.offsetHeight - markerHeight};
	}
	_onMove({x, y})
	{
		const boundaries = this._boundaries;
		x = this._startOffsets[0] + x;
		y = this._startOffsets[1] + y;
		if (x < boundaries.xFrom)
			x = boundaries.xFrom;
		else
		{
			if (x > boundaries.xTo)
				x = boundaries.xTo;
		}
		if (y < boundaries.yFrom)
			y = boundaries.yFrom;
		else
		{
			if (y > boundaries.yTo)
				y = boundaries.yTo;
		}
		this._position=[x,y];
		this._setPosition(x, y);
	}
	_onEndMove(){
		const position = this._position;
		const latLng = this._pixelOffsetToLatLng((this._element.offsetWidth / 2) + position[0], this._element.offsetHeight + position[1]);
		
			this._propertyBindingLatLng.set(latLng);
			this._getMap().getView().setZoom(16);
			this._setPosition(null, null);
	}
	_setPosition(x, y)
	{
		if(isNullOrUndefined(x))
			this._locationPickerElement.classList.remove('moving-marker');
		else
			this._locationPickerElement.classList.add('moving-marker');
		this._element.style.left = isNullOrUndefined(x)?null:(String(x) + 'px');
		this._element.style.top =  isNullOrUndefined(y)?null:(String(y) + 'px');
	}
	_pixelOffsetToLatLng(x, y) {
		const mapElement = this._mapElement;
		const map = this._getMap();
		const proportionX = (x - mapElement.offsetLeft)/mapElement.offsetWidth;
		const proportionY = (y - mapElement.offsetTop)/mapElement.offsetHeight;
		const bounds = map.getView().calculateExtent(map.getSize());
		const showingDimensions = [bounds[2]-bounds[0], bounds[3]-bounds[1]];
		const worldCoordinatesFrom = [bounds[0], bounds[3]];
		const worldCoordinateNewCenter = [
			worldCoordinatesFrom[0] +(proportionX*showingDimensions[0]),
			worldCoordinatesFrom[1] - (proportionY*showingDimensions[1])
		];
		const lonLat = toLonLat(worldCoordinateNewCenter);
		const latLng = {lat:lonLat[1], lng:lonLat[0]};
		return latLng
	}
	_latLngChanged(value){
		if(value){
			this._element.classList.remove('unset');
		}
		else
			this._element.classList.add('unset');
	}
}
	