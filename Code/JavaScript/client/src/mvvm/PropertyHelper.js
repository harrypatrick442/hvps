import capitalizeFirstLetter from '../core/capitalizeFirstLetter';
export default class PropertyHelper{
	//type '+model.constructor.name+' does not contain a getter method with name '+getterName);
	static getGetterName(propertyName){
		return PropertyHelper._getGetterName(capitalizeFirstLetter(propertyName));
	}
	static getSetterName(propertyName){
		return PropertyHelper._getSetterName(capitalizeFirstLetter(propertyName));
	}
	static hasGetter(model, propertyName){
		return model[PropertyHelper._getGetterName(capitalizeFirstLetter(propertyName))]?true:false;
	}
	static propertyValuesToObj(obj, model, propertyNames){
		if(!obj)obj={};
		propertyNames.forEach((propertyName)=>{
			var getterName = PropertyHelper._getGetterName(capitalizeFirstLetter(propertyName));
			var method = model[getterName];
			if(method===undefined)throw new Error('Model of type '+model.constructor.name+' has no method '+getterName);
			obj[propertyName]=method();
		});
		return obj;
	}
	static propertyValuesFromObj(obj, model, propertyNames){
		propertyNames.forEach((propertyName)=>{
			var setterName = PropertyHelper._getSetterName(capitalizeFirstLetter(propertyName));
			var method = model[setterName];
			if(method===undefined)throw new Error('Model of type '+model.constructor.name+' has no method '+setterName);
			method(obj[propertyName]);
		});
		return obj;
	}
	static _getGetterName(capitalizedName){
		return 'get'+capitalizedName;
	}
	static _getSetterName(capitalizedName){
		return 'set'+capitalizedName;
	}
}