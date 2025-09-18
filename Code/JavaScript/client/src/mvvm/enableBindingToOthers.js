import MyBindings from './MyBindings';
export default function enableBindingToOthers (me){
	if(me.myBindings)return me.myBindings;
	me.myBindings= new MyBindings(me);
	return me.myBindings;
}