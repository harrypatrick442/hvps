import i from '../../ui_core/i';
import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import OrderedItems from '../../tippler_ui/OrderedItems';
import NativeAPI from '../../api/NativeAPI';
import E from '../../ui_core/E';
import _createImageHoverButton from '../../ui_core/_createImageHoverButton';
import _createImageHoverTextButton from '../../ui_core/_createImageHoverTextButton';
import _createTextButton from '../../ui_core/_createTextButton';
import ResizeWatchers from '../../ui_core/ResizeWatchers';
import isMobile from '../../core/isMobile';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import Console from '../../components/Console';
import State from './State';
import ValueAndMaxField from './ValueAndMaxField';
import SystemState from '../../enums/SystemState';
import BluetoothDevicePicker from './BluetoothDevicePicker';
import Blocker from '../../tippler_ui/Blocker';
import './HVPSUI.scss';
export default class HVPSUI{
	constructor({model}){
		const disposes=[];
		this._disposes = disposes;
		this._stateChanged = this._stateChanged.bind(this);
		this._showBluetoothReconnectChanged = this._showBluetoothReconnectChanged.bind(this);
		this._autoScrollingChanged = this._autoScrollingChanged.bind(this);
		this._getFullClassName = this._getFullClassName.bind(this);
		this.dispose = this.dispose.bind(this);
		this._element = document.getElementById('root');
		this._element.classList.add('hvpsui');
		this._innerElement = E.div('inner');
		this._state = new State({propertyName:'state', model});
		PropertyBindingFactory.standard(this, model, 'state', this._stateChanged);
		this._element.appendChild(this._state.element);
		this._element.appendChild(this._innerElement);
		this._bluetoothDevicePicker = new BluetoothDevicePicker({
			model, 
			propertyNameBusy:"bluetoothBusy",
			propertyNameRefreshing:'refreshingBluetooth'
		});
		this._innerElement.appendChild(this._bluetoothDevicePicker.element);
		this._console = new Console({
			model, 
			eventNameAppendLine:'consoleAppendLine', 
			eventNameClear:'consoleClear',
			maxNLines:500,
			propertyNameAutoScrolling:'autoScrolling'
		});
		const controlButtonsElement = E.div('control-buttons');
		this._deviceSpecificElement = E.div('device-specific');
		this._innerElement.appendChild(this._deviceSpecificElement);
		this._deviceSpecificElement.appendChild(controlButtonsElement);
		disposes.push(this._console.dispose);
		[
			_createTextButton({className:'start-button', callback:model.start, disposes, useMouseDown:true, text:'Start'}),
			_createTextButton({className:'stop-button', callback:model.stop, disposes, useMouseDown:true, text:'Stop'}),
			_createTextButton({className:'shut-down-button', callback:model.shutDown, disposes, useMouseDown:true, text:'Shut Down'}),
			_createTextButton({className:'run-system-checks-only-button', callback:model.runSystemChecksOnly, disposes, useMouseDown:true, text:'Run System Checks Only'}),
			_createTextButton({className:'clear-logged-errors-on-device-button', callback:model.clearLoggedErrorsOnDevice, disposes, useMouseDown:true, text:'Clear Logged Error On Device'}),
		].forEach(b=>controlButtonsElement.appendChild(b));
		const fieldsElement = E.div('fields');
		this._fields = [
			new ValueAndMaxField({
				text:'Output Voltage',
				units:'V',
				className:'output-voltage-field', 
				propertyNameValue:'outputVoltageVolts', 
				propertyNameMax:'outputVoltageVoltsMax',
				propertyNameValueBoundType:'outputVoltageValueBoundType',
				model
			}),/*
			new ValueAndMaxField({
				text:'Output Current',
				units:'A',
				className:'output-current-field', 
				propertyNameValue:'outputCurrentAmps', 
				propertyNameMax:'outputCurrentAmpsMax', 
				model
			}),
			new ValueAndMaxField({
				text:'Output Power',
				units:'W',
				className:'output-power-field', 
				propertyNameValue:'outputPowerWatts', 
				propertyNameMax:'outputPowerWattsMax',
				model
			}),
			new ValueAndMaxField({
				text:'Total Output Energy',
				units:'J',
				className:'total-output-energy-field', 
				propertyNameValue:'totalOutputEnergyJouls',
				model
			}),*/
			new ValueAndMaxField({
				text:'First Stage Voltage',
				units:'V',
				className:'first-stage-voltage-field', 
				propertyNameValue:'firstStageVoltageVolts', 
				propertyNameMax:'firstStageVoltageVoltsMax',
				propertyNameValueBoundType:'firstStageVoltageValueBoundType',
				model
			}),
			new ValueAndMaxField({
				text:'Peak Primary Current',
				units:'A',
				className:'peak-primary-current-field', 
				propertyNameValue:'peakPrimaryCurrentAmps', 
				propertyNameMax:'peakPrimaryCurrentAmpsMax',
				propertyNameValueBoundType:'peakPrimaryCurrentValueBoundType',
				model
			}),
			new ValueAndMaxField({
				text:'Frequency',
				units:'Hz',
				className:'frequency-field', 
				propertyNameValue:'frequencyHz', 
				propertyNameMax:'frequencyHzMax',
				model
			}),
			new ValueAndMaxField({
				text:'MOSFET Temperature',
				units:'°C',
				className:'mosfet-temperature-field', 
				propertyNameValue:'mosfetTemperatureDegreesC', 
				propertyNameMax:'mosfetTemperatureDegreesCMax',
				model
			}),
			new ValueAndMaxField({
				text:'Lower Snubber Diode Temperature',
				units:'°C',
				className:'lower-snubber-diode-temperature-field', 
				propertyNameValue:'lowerSnubberDiodeTemperatureDegreesC', 
				propertyNameMax:'lowerSnubberDiodeTemperatureDegreesCMax',
				model
			})
		];
		this._fields.forEach(v=>{
			disposes.push(v.dispose);
			fieldsElement.appendChild(v.element);
		});
		this._consoleWrapperElement = E.div('console-wrapper');
		const clearConsoleButton = _createImageHoverButton(
			i('BinRed'), i('BinLidLiftedRed'), 
			'clear-console-button', 
			model.clearConsole, disposes);
		const startAutoScrollButton = _createImageHoverButton(
			i('StartAutoScrollBlue'), i('StartAutoScrollWhite'), 
			'start-auto-scroll-button', 
			model.startAutoScrolling, disposes);
		const stopAutoScrollButton = _createImageHoverButton(
			i('StopAutoScrollRed'), i('StopAutoScrollWhite'), 
			'stop-auto-scroll-button', 
			model.stopAutoScrolling, disposes);
			
		this._deviceSpecificElement.appendChild(fieldsElement);
		this._consoleWrapperElement.appendChild(clearConsoleButton);
		this._consoleWrapperElement.appendChild(startAutoScrollButton);
		this._consoleWrapperElement.appendChild(stopAutoScrollButton);
		this._innerElement.appendChild(this._consoleWrapperElement);
		this._consoleWrapperElement.appendChild(this._console.element);
		this._disconnectedBlocker = new Blocker({
			model, 
			propertyNameBlocking:'bluetoothDisconnected'
		});
		this._deviceSpecificElement.appendChild(this._disconnectedBlocker.element);
		const reconnectDialogElement = E.div('reconnect-dialog');
		const textDisconnectedElement = E.div('text');
		reconnectDialogElement.appendChild(textDisconnectedElement);
		const buttonReconnect = _createImageHoverTextButton(i('Connect'), 
			i('ConnectHover'), 'reconnect-button', model.reconnect, disposes, 'Reconnect', true);
		reconnectDialogElement.appendChild(buttonReconnect);
		this._deviceSpecificElement.appendChild(reconnectDialogElement);
		PropertyBindingFactory.standard(this, model, 'showBluetoothReconnect', this._showBluetoothReconnectChanged);
		PropertyBindingFactory.standard(this, model, 'autoScrolling', this._autoScrollingChanged);
	}
	_stateChanged(value){
		console.log('_stateChanged');
		console.log(value);
		if(!isNullOrUndefined(this._currentState)){
			this._element.classList.remove(this._getFullClassName(this._currentState));
		}
		this._currentState = value;
		if(isNullOrUndefined(value)){
			return;
		}
		this._element.classList.add(this._getFullClassName(value));
	}
	_getFullClassName(systemState){
		return `state-${SystemState.getClassName(systemState)}`;
	}
	_showBluetoothReconnectChanged(value){
		if(value){
			this._element.classList.add('show-bluetooth-reconnect');
			return;
		}
		this._element.classList.remove('show-bluetooth-reconnect');
	}
	_autoScrollingChanged(value){
		if(value){
			this._consoleWrapperElement.classList.add('auto-scrolling');
			return;
		}
		this._consoleWrapperElement.classList.remove('auto-scrolling');
	}
	dispose(){
		this._disposes.forEach(d=>d());
	}
}