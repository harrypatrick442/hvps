import exposeBinding from './exposeBinding';
export default class ExposedBindingsUsingMap{
constructor({self, properties}){
		if(!self)throw new Error('self');
		this._self = self;
		this._properties = properties;
		this.clearChanged = this.clearChanged.bind(this);
		this.getValue = this.getValue.bind(this);
		this.setValue = this.setValue.bind(this);
		this.setProperties = this.setProperties.bind(this);
		this._setPropertyValue = this._setPropertyValue.bind(this);
		this.getChanged = this.getChanged.bind(this);
		this.add = this.add.bind(this);
		this._mapPropertyNameToState= new Map();
		this.getProperty=(name)=>this._mapPropertyNameToState.get(name);
		properties.forEach(this.add);
	}
	getChanged(){
		return this._properties.filter(property=>property.startValue!==property.value);
	}
	setProperties(properties){
		const mapPropertyNameToState = this._mapPropertyNameToState;
		properties.forEach(property=>{
			const {name, value, startValue}=property;
			const existingProperty = mapPropertyNameToState.get(property.name);
			if(!existingProperty){
				//this.add(property);
				console.error(name+' is not in properties');
				return;
			}
			const changedchanged = existingProperty.value!==value;
			existingProperty.startValue = value;
			this._setPropertyValue(existingProperty, value);
		});
	}
	getValue(name){
		return this.getProperty(name).value;
	}
	setValue(name, value, asStartValue){
		const property = this.getProperty(name);
		if(!property)return;
		if(asStartValue)
			property.startValue = value;
		this._setPropertyValue(property, value);
	}
	clearChanged(){
		this._properties.forEach(property=>property.startValue=property.value);
	}
	add(property){
		const {name, onChanges, value} = property;
		property.startValue = value;
		this._mapPropertyNameToState.set(name, property);
		exposeBinding(
			this._self, 
			name,
			()=>property.value, 
			(value)=>this._setPropertyValue(property, value),
			true
		);
	}
	_setPropertyValue(property, value){
		const existingValue = property.value;
		if(value===existingValue)return;
		property.value = value;
		this._self.bindingsHandler.changed(property.name, value);
		property.onChanges?.forEach(c=>c(value));
	}
}