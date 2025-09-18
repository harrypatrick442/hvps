import BindingsHandler from './BindingsHandler';
export default function enableAcceptBindings(model){
	if(model.bindingsHandler)
		return model.bindingsHandler;
	model.bindingsHandler = new BindingsHandler(model);
	return model.bindingsHandler;
};