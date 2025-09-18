export default function exposeMethod(model, name, method){
	model[name]=method;
}