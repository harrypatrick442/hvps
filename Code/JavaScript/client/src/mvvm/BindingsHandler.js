import PropertyBindings from './PropertyBindings';
export default class BindingsHandler{
	constructor(model){
		this._model = model;
		this._mapNameToPropertyBindings  = new Map()
		this.addPropertyBinding = this.addPropertyBinding.bind(this);
		this.changed = this.changed.bind(this);
		this.dispose = this.dispose.bind(this);
		this.getObjectsBoundToMe = this.getObjectsBoundToMe.bind(this);
		this._handlePropertyBindingsDispose = this._handlePropertyBindingsDispose.bind(this);
	}
	has(name){
		return this._mapNameToPropertyBindings.has(name);
	}
	addPropertyBinding(propertyBinding){
		const name = propertyBinding.getName();
		let propertyBindings = this._mapNameToPropertyBindings.get(name);
		if(!propertyBindings){
			propertyBindings = new PropertyBindings(name, propertyBinding);
			propertyBindings.addEventListener('dispose', this._handlePropertyBindingsDispose);
			this._mapNameToPropertyBindings.set(name, propertyBindings);
			return;
		}
		if(propertyBindings.containsCallbackValueChanged(propertyBinding.callbackValueChanged))
			return false;
		propertyBindings.add(propertyBinding);
	}
	changed(name, value){
		const propertyBindings =  this._mapNameToPropertyBindings.get(name);
		if(!propertyBindings)return;
		propertyBindings.changed(value);
	}
	dispose(){
		for(let propertyBinding of Array.from(this._mapNameToPropertyBindings.values())){
			propertyBinding.dispose();
		}
		//this._mapNameToPropertyBindings.clear();
	}
	getObjectsBoundToMe(){
		const mes=[];
		this._mapNameToPropertyBindings.values().forEach(propertyBindings=>{
			for(var propertyBinding of propertyBindings.getList()){
				if(mes.indexOf(propertyBinding.me)<0)
					mes.push(propertyBinding.me);
			}
		});
		return mes;
	}
	_handlePropertyBindingsDispose({propertyBindings}){
		this._mapNameToPropertyBindings.delete(propertyBindings.getName());
	}
}