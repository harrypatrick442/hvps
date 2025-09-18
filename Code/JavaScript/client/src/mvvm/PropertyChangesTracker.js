import PropertyBinding from './PropertyBinding';
export default class PropertyChangesTracker{
	constructor (model, propertyNames, propertyChanged){
		this._mapPropertyNameToInitialValue=new Map();
		this._propertyBindings=[];
		this._propertyChanged=propertyChanged;
		this.getChanged = this.getChanged.bind(this);
		this.reset = this.reset.bind(this);
		this._mapInitialValues=this._mapInitialValues.bind(this);
		propertyNames.forEach((propertyName)=>{
			const loopBreak = new LoopBreak();
			const propertyBinding = PropertyBinding.standard(model, model, propertyName, (value)=>{
				if(loopBreak.trigger())return;
				_propertyChanged&&_propertyChanged(propertyName, value);
			});
			this._propertyBindings.push(propertyBinding);
		});
		this._mapInitialValues();
	}
	getChanged(){
		for(var i=0; i<propertyBindings.length; i++){
			const propertyBinding = this._propertyBindings[i];
			const propertyName = propertyBinding.getName();
			const initialValue = this._mapPropertyNameToInitialValue.get(propertyName);
			const currentValue = propertyBinding.get();
			if(initialValue!==currentValue){
				//console.log('property '+propertyName+' changed from '+initialValue+' to '+currentValue);
				return true;
			}
		}
		return false;
	};
	reset(){
		this._mapInitialValues();
	}
	_mapInitialValues(){
		propertyBindings.forEach((propertyBinding)=>{
			this._mapPropertyNameToInitialValue.get(propertyBinding.getName()]=propertyBinding.get());
		});
	}
}