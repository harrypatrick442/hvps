export default class MyBindings{
	constructor(me){
		this._propertyBindings = new Set();
		this._disposed=false;
		this.add = this.add.bind(this);
		this.dispose = this.dispose.bind(this);
		this.reset = this.reset.bind(this);
		this._handlePropertyBindingDispose = this._handlePropertyBindingDispose.bind(this);
	}
	get propertyBindings(){
		return this._propertyBindings;
	}
	add(propertyBinding){
		if(this._propertyBindings.has(propertyBinding))
			return;
		this._propertyBindings.add(propertyBinding);
		propertyBinding.addEventListener('dispose', this._handlePropertyBindingDispose);
	}
	dispose(){	
		if(this._disposed)
			return;
		this._disposed=true;
		this.reset();
	}
	reset(){
		this._propertyBindings.forEach((propertyBinding)=>{
			propertyBinding.dispose();
		});
		this._propertyBindings=new Set();
	}
	_handlePropertyBindingDispose({propertyBinding}){
		this._propertyBindings.delete(propertyBinding);
	}
}