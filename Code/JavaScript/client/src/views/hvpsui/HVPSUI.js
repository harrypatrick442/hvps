import i from '../../ui_core/i';
import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import OrderedItems from '../../tippler_ui/OrderedItems';
import NativeAPI from '../../api/NativeAPI';
import E from '../../ui_core/E';
import _createImageHoverTextButton from '../../ui_core/_createImageHoverTextButton';
import _createTextButton from '../../ui_core/_createTextButton';
import ResizeWatchers from '../../ui_core/ResizeWatchers';
import isMobile from '../../core/isMobile';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import Console from '../../components/Console';
import State from './State';
import ValueAndMaxField from './ValueAndMaxField';
import HVPSState from '../../enums/HVPSState';
import BluetoothDevicePicker from './BluetoothDevicePicker';
import Blocker from '../../tippler_ui/Blocker';
import './HVPSUI.scss';
export default class HVPSUI{
	constructor({model}){
		const disposes=[];
		this._disposes = disposes;
		this._stateChanged = this._stateChanged.bind(this);
		this._showBluetoothReconnectChanged = this._showBluetoothReconnectChanged.bind(this);
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
			maxNLines:30
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
		].forEach(b=>controlButtonsElement.appendChild(b));
		const fieldsElement = E.div('fields');
		this._fields = [
			new ValueAndMaxField({
				text:'Output Voltage',
				units:'V',
				className:'output-voltage-field', 
				propertyNameValue:'outputVoltage', 
				propertyNameMax:'outputVoltageMax', 
				model
			}),
			new ValueAndMaxField({
				text:'Output Current',
				units:'A',
				className:'output-current-field', 
				propertyNameValue:'outputCurrent', 
				propertyNameMax:'outputCurrentMax', 
				model
			}),
			new ValueAndMaxField({
				text:'Output Power',
				units:'W',
				className:'output-power-field', 
				propertyNameValue:'outputPower', 
				propertyNameMax:'outputPowerMax',
				model
			}),
			new ValueAndMaxField({
				text:'Total Output Energy',
				units:'J',
				className:'total-output-energy-field', 
				propertyNameValue:'totalOutputEnergy', 
				propertyNameMax:'totalOutputEnergyMax',
				model
			}),
			new ValueAndMaxField({
				text:'First Stage Voltage',
				units:'V',
				className:'first-stage-voltage-field', 
				propertyNameValue:'firstStageVoltage', 
				propertyNameMax:'firstStageVoltageMax',
				model
			}),
			new ValueAndMaxField({
				text:'Peak Primary Current',
				units:'A',
				className:'peak-primary-current-field', 
				propertyNameValue:'peakPrimaryCurrent', 
				propertyNameMax:'peakPrimaryCurrentMax',
				model
			})
		];
		this._fields.forEach(v=>{
			disposes.push(v.dispose);
			fieldsElement.appendChild(v.element);
		});
		this._deviceSpecificElement.appendChild(fieldsElement);
		this._innerElement.appendChild(this._console.element);
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
	}
	_stateChanged(hvpsState){
		if(!isNullOrUndefined(this._currentState)){
			this._element.classList.remove(`state-${HVPSState.getClassName(hvpsState)}`);
		}
		this._currentState = hvpsState;
		if(isNullOrUndefined(hvpsState)){
			return;
		}
		this._element.classList.add(`state-${HVPSState.getClassName(hvpsState)}`);
	}
	_showBluetoothReconnectChanged(value){
		if(value){
			this._element.classList.add('show-bluetooth-reconnect');
			return;
		}
		this._element.classList.remove('show-bluetooth-reconnect');
	}
	dispose(){
		this._disposes.forEach(d=>d());
	}
}