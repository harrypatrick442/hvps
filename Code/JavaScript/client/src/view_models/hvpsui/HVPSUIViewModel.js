import PropertyBindingFactory from '../../mvvm/PropertyBindingFactory';
import exposeBinding  from '../../mvvm/exposeBinding';
import exposeMethod  from '../../mvvm/exposeMethod';
import UrlParameters from '../../urls/UrlParameters';
import NativeAPI  from '../../api/NativeAPI';
import eventEnable  from '../../core/eventEnable';
import isNullOrUndefined from '../../core/isNullOrUndefined';
import isNullOrUndefinedOrEmptyString  from '../../core/isNullOrUndefinedOrEmptyString';
import HVPSState  from '../../enums/HVPSState';
import HVPSUIAPI  from '../../api/HVPSUIAPI';
import Handles  from '../../core/Handles';
import BluetoothFailedReason  from '../../enums/BluetoothFailedReason';
import HVPSUIDialog from '../../components/hvpsui/HVPSUIDialog';
import ConsoleMessageType from '../../enums/ConsoleMessageType';
import SubsystemIdentifier from '../../enums/SubsystemIdentifier';
import ErrorViewModel from './ErrorViewModel';
export default class HVPSUIViewModel{
	constructor(){
		eventEnable(this);
		const disposes = [];
		this._disposes = disposes;
		this.start = this.start.bind(this);
		this.stop = this.stop.bind(this);
		this.shutDown = this.shutDown.bind(this);
		this.runSystemChecksOnly = this.runSystemChecksOnly.bind(this);
		this.clearLoggedErrorsOnDevice = this.clearLoggedErrorsOnDevice.bind(this);
		this._connectToBluetoothDevice = this._connectToBluetoothDevice.bind(this);
		this._setBluetoothConnected = this._setBluetoothConnected.bind(this);
		this._setShowBluetoothReconnect = this._setShowBluetoothReconnect.bind(this);
		this._setRefreshingBluetooth = this._setRefreshingBluetooth.bind(this);
		this._handleConsoleMessage = this._handleConsoleMessage.bind(this);
		this._handleErrorMessage = this._handleErrorMessage.bind(this);
		this._consoleClear = this._consoleClear.bind(this);
		this._consoleAppendLine = this._consoleAppendLine.bind(this);
		this.refreshBluetoothDevices = this.refreshBluetoothDevices.bind(this);
		this.reconnect = this.reconnect.bind(this);
		this._setDevices = this._setDevices.bind(this);
		this._handleDisconnected = this._handleDisconnected.bind(this);
		this._handleCoreDumpSummaryMessage = this._handleCoreDumpSummaryMessage.bind(this);
		this._handleLastAbortMessage = this._handleLastAbortMessage.bind(this);
		this._buildUsefulCoreDumpSummaryLines = this._buildUsefulCoreDumpSummaryLines.bind(this);
		this._notSelectedDevice = {name:'Not Selected', address:''};
		this._state = HVPSState.Unknown;
		this._devices = [
			this._notSelectedDevice
		];
		this._bluetoothConnected = false;
		this._showBluetoothReconnect = false;
		this._selectedDevice = this._notSelectedDevice;
		this._bluetoothBusyHandles = new Handles({onChange:(hasHandles)=>{
			this.bindingsHandler.changed('bluetoothBusy', hasHandles);
		}});
		exposeBinding(this, 'devices', ()=>this.devices);
		exposeBinding(this, 'selectedDevice', ()=>this.selectedDevice, (value)=>this.selectedDevice = value);
		exposeBinding(this, 'state', ()=>this.state);
		exposeBinding(this, 'outputVoltage', ()=>this.outputVoltage);
		exposeBinding(this, 'outputVoltageMax', ()=>this.outputVoltageMax);
		exposeBinding(this, 'outputCurrent', ()=>this.outputCurrent);
		exposeBinding(this, 'outputCurrentMax', ()=>this.outputCurrentMax);
		exposeBinding(this, 'outputPower', ()=>this.outputPower);
		exposeBinding(this, 'outputPowerMax', ()=>this.outputPowerMax);
		exposeBinding(this, 'totalOutputEnergy', ()=>this.totalOutputEnergy);
		exposeBinding(this, 'totalOutputEnergyMax', ()=>this.totalOutputEnergyMax);
		exposeBinding(this, 'firstStageVoltage', ()=>this.firstStageVoltage);
		exposeBinding(this, 'firstStageVoltageMax', ()=>this.firstStageVoltageMax);
		exposeBinding(this, 'peakPrimaryCurrent', ()=>this.peakPrimaryCurrent);
		exposeBinding(this, 'peakPrimaryCurrentMax', ()=>this.peakPrimaryCurrentMax);
		exposeBinding(this, 'bluetoothBusy', ()=>this.bluetoothBusy);
		exposeBinding(this, 'refreshingBluetooth', ()=>this.refreshingBluetooth);
		exposeBinding(this, 'bluetoothConnected', ()=>this.bluetoothConnected);
		exposeBinding(this, 'bluetoothDisconnected', ()=>this.bluetoothDisconnected);
		exposeBinding(this, 'showBluetoothReconnect', ()=>this.showBluetoothReconnect);
		disposes.push(HVPSUIAPI.addEventListener('disconnected', this._handleDisconnected));
		disposes.push(HVPSUIAPI.addEventListener('consoleMessage', this._handleConsoleMessage));
		disposes.push(HVPSUIAPI.addEventListener('errorMessage', this._handleErrorMessage));
		disposes.push(HVPSUIAPI.addEventListener('coreDumpSummaryMessage', this._handleCoreDumpSummaryMessage));
		disposes.push(HVPSUIAPI.addEventListener('lastAbortMessage', this._handleLastAbortMessage));
		this.refreshBluetoothDevices();
		
	}
	get state(){
		return this._state;
	}
	get devices(){
		return this._devices;
	}
	_setDevices(value){
		this._devices = value;
		this.bindingsHandler.changed('devices', value);
	}
	get selectedDevice(){
		return this._selectedDevice;
	}
	set selectedDevice(value){
		if(isNullOrUndefinedOrEmptyString(value)){
			if(value===this._selectedDevice){
				return;
			}
			this._connectToBluetoothDevice(null);
			this._setShowBluetoothReconnect(false);
		}
		else{
			if(!isNullOrUndefined(this._selectedDevice)){
				if(value.address === this._selectedDevice.address){
					return;
				}
			}
			this._setBluetoothConnected(false);
			this._connectToBluetoothDevice(value.address);
		}
		this._selectedDevice = value;
		this.bindingsHandler.changed('selectedDevice', value);
	}
	get bluetoothBusy(){
		return this._bluetoothBusyHandles.taken;
	}
	get refreshingBluetooth(){
		return this._refreshingBluetooth;
	}
	_setRefreshingBluetooth(value){
		console.log('_setRefreshingBluetooth');
		if(this._refreshingBluetooth===value)return;
		this._refreshingBluetooth = value;
		this.bindingsHandler.changed('refreshingBluetooth', value);
	}
	get bluetoothConnected(){
		return this._bluetoothConnected;
	}
	get bluetoothDisconnected(){
		return !this._bluetoothConnected;
	}
	_setBluetoothConnected(value){
		console.log('_setBluetoothConnected '+value);
		if(value===this._bluetoothConnected)return;
		this._bluetoothConnected = value;
		this.bindingsHandler.changed('bluetoothConnected', value);
		this.bindingsHandler.changed('bluetoothDisconnected', !value);
	}
	get showBluetoothReconnect(){
		return this._showBluetoothReconnect;
	}
	get outputVoltage(){
		return this._outputVoltage;
	}
	get outputVoltageMax(){
		return this._outputVoltageMax;
	}
	get outputCurrent(){
		return this._outputCurrent;
	}
	get outputCurrentMax(){
		return this._outputCurrentMax;
	}
	get outputPower(){
		return this._outputPower;
	}
	get outputPowerMax(){
		return this._outputPowerMax;
	}
	get totalOutputEnergy(){
		return this._totalOutputEnergy;
	}
	get totalOutputEnergyMax(){
		return this._totalOutputEnergyMax;
	}
	get firstStageVoltage(){
		return this._firstStageVoltage;
	}
	get firstStageVoltageMax(){
		return this._firstStageVoltageMax;
	}
	get peakPrimaryCurrent(){
		return this._peakPrimaryCurrent;
	}
	get peakPrimaryCurrentMax(){
		return this._peakPrimaryCurrentMax;
	}
	start(){
		HVPSUIAPI.start();
	}
	stop(){
		HVPSUIAPI.stop();
	}
	shutDown(){
		HVPSUIAPI.shutDown();
	}
	refreshBluetoothDevices(){
		var busyHandle = this._bluetoothBusyHandles.take();
		this._setRefreshingBluetooth(true);
		try{
			HVPSUIAPI.getAvailableBluetoothDevices()
			.then(res=>{
				console.log('res was: ');
				console.log(res);
				res.devices.splice(0, 0, this._notSelectedDevice);
				this._setDevices(res.devices);
			}).catch(err=>{
				console.error(err);
			}).finally(()=>{
			this._setRefreshingBluetooth(false);
				busyHandle.dispose();
			});
		}
		catch(err){
		this._setRefreshingBluetooth(false);
			busyHandle.dispose();
		}
	}
	runSystemChecksOnly(){
		HVPSUIDialog.show({
			options:[
				{
					text:'OK Run', 
					callback:HVPSUIAPI.runSystemChecksOnly
				},
				{
					text:'Do Not Run', 
					callback:()=>{}
				}
			],
			message:'The device will go LIVE while running system checks and must be treated as LIVE!',
			title:'DANGER'
		}
		);
	}
	clearLoggedErrorsOnDevice(){
		HVPSUIDialog.show({
			options:[
				{
					text:'OK Clear', 
					callback:HVPSUIAPI.clearLoggedErrors
				},
				{
					text:'Do Not Clear', 
					callback:()=>{}
				}
			],
			message:'Are you sure you wish to clear logged errors from the device?',
			title:'DANGER'
		}
		);
	}
	reconnect(){
		const address = this.selectedDevice.address;
		if(isNullOrUndefinedOrEmptyString(address))return;
		this._connectToBluetoothDevice(address);
	}
	test(){
		HVPSUIAPI.test();
	}
	_connectToBluetoothDevice(address){
		var busyHandle = this._bluetoothBusyHandles.take();
		try{
			HVPSUIAPI.connectToBluetoothDevice(address)
			.then(res=>{
				console.log(res);
				if(isNullOrUndefined(res.failedReason)){
					this._setShowBluetoothReconnect(false);
					this._setBluetoothConnected(true);
					return null;
				}
				this._setShowBluetoothReconnect(true);
				return res.failedReason;
			})
			.catch(err=>{
				console.error(err);
				this._setShowBluetoothReconnect(true);
				return BluetoothFailedReason.Unknown;
			}).finally((res)=>{
				busyHandle.dispose();
				return res;
			});
		}
		catch(err){
			busyHandle.dispose();
		}
	}
	
	_setShowBluetoothReconnect(value){
		if(this._showBluetoothReconnect===value)return;
		this._showBluetoothReconnect= value;
		this.bindingsHandler.changed('showBluetoothReconnect', value);
	}
	_handleDisconnected(){
		this._setBluetoothConnected(false);
		this._setShowBluetoothReconnect(true);
	}
	_handleConsoleMessage({consoleMessage}){
		console.log('_handleConsoleMessage');
		this._consoleAppendLine(
			consoleMessage.message,
			consoleMessage.isError?ConsoleMessageType.Error:ConsoleMessageType.Info);
	}
	_handleErrorMessage({errorMessage}){
		this.dispatchEvent({type:'error', error:new ErrorViewModel({errorMessage})});
	}
	_handleCoreDumpSummaryMessage({coreDumpSummaryMessage}){
		this._consoleAppendLine('Core Dump Summary: ', ConsoleMessageType.Error);
		const lines = this._buildUsefulCoreDumpSummaryLines(coreDumpSummaryMessage);
		lines.forEach(line=>this._consoleAppendLine(line, ConsoleMessageType.Error));
		this._consoleAppendLine();
		this._consoleAppendLine();
	}
	_handleLastAbortMessage({lastAbortMessage}){
		this._consoleAppendLine(`Last Abort Reason: ${lastAbortMessage.reason}`, ConsoleMessageType.Error);
		if(lastAbortMessage.backtrace){
			lastAbortMessage.backtrace.forEach(b=>{
				this._consoleAppendLine(`0x${b.toString(16).padStart(8, "0")}`, ConsoleMessageType.Error);
			});
		}
		this._consoleAppendLine();
		this._consoleAppendLine();
	}
	_consoleClear(){
		this.dispatchEvent({type:'consoleClear'});
	}
	_consoleAppendLine(str, consoleMessageType){
		console.log('_consoleAppendLine');
		this.dispatchEvent({type:'consoleAppendLine', str, consoleMessageType});
	}
	_buildUsefulCoreDumpSummaryLines(summary) {
		const lines = [];

		if (!summary) {
			lines.push("There was no crash dump summary?");
			return lines;
		}

		const pc = summary.programCounterForException || 0;
		lines.push(`Crash subsystem: ${SubsystemIdentifier.getDescription(summary.subsystemIdentifier)}, task: ${summary.taskName || "?"}  PC=0x${pc.toString(16).padStart(8, "0")}`);

		const depth = summary.backtrace?.length || 0;
		const corrupted = summary.backtraceCorrupted ? 1 : 0;
		lines.push(`Backtrace depth=${depth}  corrupted=${corrupted}`);

		if (Array.isArray(summary.backtrace)) {
			summary.backtrace.forEach((addr, i) => {
				lines.push(`  #${i}  0x${(addr || 0).toString(16).padStart(8, "0")}`);
			});
		}

		if (summary.causeOfException !== undefined)
			lines.push(`Cause of exception: 0x${summary.causeOfException.toString(16)}`);

		if (Array.isArray(summary.aRegisterSetWhenTheExceptionCaused))
			lines.push(
				`A registers: ${summary.aRegisterSetWhenTheExceptionCaused
					.map(x => "0x" + x.toString(16).padStart(8, "0"))
					.join(", ")}`
			);

		if (Array.isArray(summary.pCRegisterAddressAtExceptionLevel1To7))
			lines.push(
				`EPCx registers: ${summary.pCRegisterAddressAtExceptionLevel1To7
					.map(x => "0x" + x.toString(16).padStart(8, "0"))
					.join(", ")}`
			);

		lines.push(`Version: ${summary.version}`);
		lines.push(`Task pointer: 0x${summary.taskPointer.toString(16)}`);
		lines.push(`Virtual address of exception: 0x${summary.virtualAddressOfException.toString(16)}`);
		lines.push(`App SHA256: ${summary.crashingApplicationsSHA256SumAsAString}`);

		return lines;
	}
	
}