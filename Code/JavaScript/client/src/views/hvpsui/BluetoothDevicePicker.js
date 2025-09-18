import i from '../../ui_core/i';
import Select from '../../tippler_ui/Select';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import E from '../../ui_core/E';
import spinner1 from '../../components/spinners/spinner1';
import _createImageHoverButton from '../../ui_core/_createImageHoverButton';
import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import Blocker from '../../tippler_ui/Blocker';
import './BluetoothDevicePicker.scss';
export default class BluetoothDevicePicker{
	constructor({model, propertyNameBusy, propertyNameRefreshing}){
		this._disposes = [];
		this._busyChanged = this._busyChanged.bind(this);
		this._refreshingChanged = this._refreshingChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._element = E.div('bluetooth-device-picker');
		const topRowElement = E.div('top-row');
		const titleElement = E.div('title');
		titleElement.textContent = 'Bluetooth Device';
		topRowElement.appendChild(titleElement);
		const buttonRefresh = _createImageHoverButton(
			i('Reload'), i('ReloadHover'), 'refresh-bluetooth-devices-button', 
			model.refreshBluetoothDevices, this._disposes, true);
		topRowElement.appendChild(buttonRefresh);
		this._element.appendChild(topRowElement);
		this._select = new Select({
			model, 
			className:'devices', 
			propertyNameSelectedItem:'selectedDevice', 
			propertyNameItems:'devices',
			itemTextName:'name', 
			itemValueName:'address'
		});
		this._element.appendChild(this._select.element);
		if(!isNullOrUndefined(propertyNameBusy)){
			PropertyBindingFactory.standard(this, model, propertyNameBusy, this._busyChanged);
			this._blocker = new Blocker({model, propertyNameBlocking:propertyNameBusy});
			this._element.appendChild(this._blocker.element);
			this._spinnerElement = new spinner1();
			this._element.appendChild(this._spinnerElement);
		}
		if(!isNullOrUndefined(propertyNameRefreshing)){
			PropertyBindingFactory.standard(this, model, 
				propertyNameRefreshing, this._refreshingChanged);
			
		}
	}
	get element(){
		return this._element;
	}
	_busyChanged(value){
		if(value){
			this._element.classList.add('busy');
			return;
		}
		this._element.classList.remove('busy');
	}
	_refreshingChanged(value){
		console.log('_refreshingChanged');
		if(value){
			this._element.classList.add('refreshing');
			return;
		}
		this._element.classList.remove('refreshing');
	}
	dispose(){
		this._blocker?.dispose();
		this.myBindings?.dispose();
		this._disposes.forEach(d=>d());
		this._select.dispose();
	}
}