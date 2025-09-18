import BingMaps from 'ol/source/BingMaps.js';
import Map from 'ol/Map.js';
import TileLayer from 'ol/layer/Tile.js';
import View from 'ol/View.js';
import {fromLonLat, toLonLat, transformExtent} from 'ol/proj';
import E from '../ui_core/E';
import ResizeWatchers from '../ui_core/ResizeWatchers';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import MapTextSearch from './MapTextSearch';
import GoogleMaps from '../google/GoogleMaps';
import Spinner from './Spinner';
import isNullOrUndefined from '../core/isNullOrUndefined';
import LoopBreak from '../core/LoopBreak';
import TemporalCallback from '../core/TemporalCallback';
import MapLayerStyles from '../enums/MapLayerStyles';
import Select from '../tippler_ui/Select';
import './MapBase.scss';


/*
const select = document.getElementById('layer-select');
function onChange() {
  const style = select.value;
  for (let i = 0, ii = layers.length; i < ii; ++i) {
    layers[i].setVisible(styles[i] === style);
  }
}
select.addEventListener('change', onChange);
onChange();*/




export default class MapBase{
	constructor({model, propertyNameLatLng, propertyNameBusy, propertyNameFormattedAddress,
		propertyNameAddressComponents, propertyNamePlaceName, propertyNameTextSearchExpanded,
		propertyNameLayerStyle, fixedLayerStyle}){
		this._model = model;
		this._disposes = [];
		this._propertyNameFormattedAddress = propertyNameFormattedAddress;
		this._propertyNameAddressComponents = propertyNameAddressComponents;
		this._propertyNameTextSearchExpanded= propertyNameTextSearchExpanded;
		this._propertyNamePlaceName = propertyNamePlaceName;
		this._propertyNameLatLng = propertyNameLatLng;
		this._propertyNameFormattedAddress = propertyNameFormattedAddress;
		this._latLngChanged = this._latLngChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._initializeTextSearch = this._initializeTextSearch.bind(this);
		this._addressComponentsChanged= this._addressComponentsChanged.bind(this);
		this._formattedAddressChanged= this._formattedAddressChanged.bind(this);
		this._setMapCenter= this._setMapCenter.bind(this);
		this._getMapCenter= this._getMapCenter.bind(this);
		this._setMapZoom= this._setMapZoom.bind(this);
		this._getMapZoom= this._getMapZoom.bind(this);
		this._layerStyleChanged = this._layerStyleChanged.bind(this);
		this._defaultLayerStyle = fixedLayerStyle?fixedLayerStyle:MapLayerStyles.AerialWithLabelsOnDemand;
		this._loopBreakMovingMapByLatLngChanged = new LoopBreak();
		this._element = E.div('map-base');
		this._mapElement = E.div('map');
		this._spinner = new Spinner({model, propertyNameVisible:propertyNameBusy});
		[this._spinner.element, this._mapElement]
			.forEach(e=>this._element.appendChild(e));
		this._resizeWatcher= ResizeWatchers.add({element:this._element, onResized:this._handleResized, 
			onFirstResize:this._onStartResize});
		this._temporalCallbackResize = new TemporalCallback({callback:this._resize, maxNTriggers:20, maxTotalDelay:500});
		
		this._propertyBindingLatLng = PropertyBindingFactory.standard(
			this, model, propertyNameLatLng, this._latLngChanged, true);
		this._propertyBindingAddressComponents = PropertyBindingFactory.standard(
			this, model, propertyNameFormattedAddress, this._addressComponentsChanged);
		this._propertyBindingFormattedAddress = PropertyBindingFactory.standard(
			this, model, propertyNameAddressComponents, this._formattedAddressChanged);
			
		const key = model.key;
		if(isNullOrUndefined(key))
			throw new Error('No key provided');
		this._mapLayerStyleToLayer = new Map();
		let layerStyleTextValueObjects = MapLayerStyles.getTextValueObjects();
		if(fixedLayerStyle) 
			layerStyleTextValueObjects = layerStyleTextValueObjects
				.filter(l=>l.value===fixedLayerStyle);
		this._layers = layerStyleTextValueObjects.map(({text, value})=>{
			const layer = new TileLayer({
				visible: false,
				preload: Infinity,
				source: new BingMaps({
				key,
				imagerySet: value,
			// placeholderTiles: false, // Optional. Prevents showing of BingMaps placeholder tiles
				}),
			});
			this._mapLayerStyleToLayer.set(value, layer);
			return layer;
		});
		this._mapLayerStyleToLayer.get(this._defaultLayerStyle).setVisible(true);		
        var latLng = {'lat': 55.3781, 'lng': -1.4360};
		this._map = new Map({
			target:this._mapElement,
			layers:this._layers,
			view: new View({
				center: fromLonLat([latLng.lng, latLng.lat]),
				zoom: model.zoom,
			}),
		});
		this._initializeTextSearch();
		if(propertyNameLayerStyle){
			PropertyBindingFactory.standard(this, model, propertyNameLayerStyle, this._layerStyleChanged);
			if(isNullOrUndefined(fixedLayerStyle)){
				this._selectLayerStyle = new Select({
					model, className:'layer-style',  defaultAbove: true,
					propertyNameSelectedValue:propertyNameLayerStyle,  
					items:layerStyleTextValueObjects, 
					itemTextName:'text', itemValueName:'value'
				});
				this._element.appendChild(this._selectLayerStyle.element);
			}
		}
		else
		{
			this._mapLayerStyleToLayer.get(
				this._defaultLayerStyle).setVisible(true);		
		}
	}
	get element(){
		return this._element;
	}
	_initializeTextSearch(){
		this._textSearch = new MapTextSearch({
			model:this._model,
			propertyNameFormattedAddress:this._propertyNameFormattedAddress,
			propertyNameExpanded:this._propertyNameTextSearchExpanded,
			propertyNameLatLng:this._propertyNameLatLng, 
			propertyNameAddressComponents:this._propertyNameAddressComponents,
			propertyNamePlaceName:this._propertyNamePlaceName
		});
		this._element.appendChild(this._textSearch.element);
	}
	_latLngChanged(value){
		if(this._loopBreakMovingMapByLatLngChanged.trigger())return;
		if(!value)return;
		if(!this._map){
			return;
		}
        this._setMapCenter(value);
	}
	_setMapCenter(latLng){
		this._map.getView().setCenter(fromLonLat([latLng.lng, latLng.lat]));
	}
	_getMapCenter(){
		const center = this._map.getView().getCenter();
		const lonLat = toLonLat(center);
		return {lat:lonLat[1], lng:lonLat[0]};
	}
	_setMapZoom(value){
		this._map.getView().setZoom(value);
	}
	_getMapZoom(){
		return this._map.getView().getZoom();
	}
	_addressComponentsChanged(value){
		
	}
	_formattedAddressChanged(value){
		
	}
	_layerStyleChanged(value){
		let layer = this._mapLayerStyleToLayer.get(value);
		if(isNullOrUndefined(layer))
			layer =  this._mapLayerStyleToLayer
				.get(this._defaultLayerStyle);	
		if(this._currentLayer === layer)return;
		if(!isNullOrUndefined(this._currentLayer))
			this._currentLayer.setVisible(false);
		layer.setVisible(true);
		this._currentLayer = layer;
	}
	dispose(){
		this._textSearch?.dispose();
		this._disposes.forEach(d=>d());
	}
}