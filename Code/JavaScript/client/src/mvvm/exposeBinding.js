import PropertyHelper from './PropertyHelper';
import enableAcceptBindings from './enableAcceptBindings';
export default function exposeBinding(model, name, getLambda, setLambda){
	const getterName = PropertyHelper.getGetterName(name);
	if(model[getterName]!==undefined)
		throw new Error(`getter for exposing \"${name}\" is already defined`);
	const setterName = PropertyHelper.getSetterName(name);
	if(model[setterName]!==undefined)
		throw new Error(`setter for exposing \"${name}\" is already defined`);
	model[getterName]=getLambda;
	model[setterName]=setLambda;
	enableAcceptBindings(model);
}