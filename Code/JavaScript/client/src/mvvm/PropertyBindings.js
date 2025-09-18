import eventEnable from '../core/eventEnable';
export default class PropertyBindings{
	constructor(name, propertyBinding){
		this._name = name;
		eventEnable(this);
		this._disposed = false;
		this._list = [];
		this.getList = this.getList.bind(this);
		this.containsCallbackValueChanged = this.containsCallbackValueChanged.bind(this);
		this.getByCallbackValueChanged = this.getByCallbackValueChanged.bind(this);
		this.changed = this.changed.bind(this);
		this.add = this.add.bind(this);
		this._handlePropertyBindingDispose = this._handlePropertyBindingDispose.bind(this);
		this._dispatchDispose = this._dispatchDispose.bind(this);
		this._remove = this._remove.bind(this);;
		this.dispose = this.dispose.bind(this);
		if(propertyBinding)
			this.add(propertyBinding);
	
	}
	getName(){
		return this._name;
	}
	getList(){
		return this._list;
	}
	containsCallbackValueChanged(callbackValueChanged){
		return this.getByCallbackValueChanged(callbackValueChanged)?true:false;
	}
	getByCallbackValueChanged(callbackValueChanged){
		const propertyBinding = this._list.filter(
			p=>p.callbackValueChanged===callbackValueChanged)[0];
		if(!propertyBinding)return null;
		return propertyBinding;
	}
	changed(value){
		this.getList().forEach((propertyBinding)=>{
			try{
				propertyBinding.callbackValueChanged(value);
			}
			catch(err){
				console.error(err);
			}
		});
	}
	_handlePropertyBindingDispose(e){
		this._remove(e.propertyBinding);	
	}
	add(propertyBinding){
		propertyBinding.addEventListener('dispose', this._handlePropertyBindingDispose);
		this._list.push(propertyBinding);
	}
	_remove(propertyBinding){
		var index = this._list.indexOf(propertyBinding);
		if(index<0)return;
		this._list.splice(index, 1);
		if(this._list.length>0)return;
		this.dispose();
	}
	dispose(){
		if(this._disposed)return;
		this._disposed = true;
		this._list.slice().forEach((propertyBinding)=>{/* because as we dispose them they will be calling back to remove themselves*/
			propertyBinding.dispose();
		});
		this._dispatchDispose(); //Not really needed because upon disposing the last binding the event will be dispatched.
	}
	_dispatchDispose(){
		this.dispatchEvent({type:'dispose', propertyBindings:this});
	}
}