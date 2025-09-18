export default function ModelBindingsForName(name, binding){
	const self = this;
	eventEnable(self);
	var disposed = false;
	const list = [];
	self.add = add;
	self.getList = ()=>list;
	self.dispose = dispose;
	self.remove = removeBinding;
	if(binding)
		add(binding);
	self.containsCallbackSet=function(callbackSet){
		return getByCallbackSet(callbackSet)?true:false;
	};
	self.getByCallbackSet = getByCallbackSet;
	self.changed= function(value){
		list.forEach((binding)=>{
			binding.callCallbackSet(value);
		});
	};
	function dispose(){
		if(disposed)return;
		disposed = true;
		list.slice().forEach((binding)=>{/* because as we dispose them they will be calling back to remove themselves*/
			binding.dispose();
		});
		dispatchDispose(); //Not really needed because upon disposing the last binding the event will be dispatched.
	}
	function add(binding){
		listenForDispose(binding);
		list.push(binding);
	}
	function listenForDispose(binding){
		binding.addEventListener('dispose', callbackDispose);
	}
	function callbackDispose(e){
		removeBinding(e.binding);
	}
	function removeBinding(binding){
		var index = list.indexOf(binding);
		if(index<0)return;
		list.splice(index, 1);
		if(list.length>0)return;
		dispose();
	}
	function dispatchDispose(){
		self.dispatchEvent({type:'dispose', bindings:self});
	}
	function getByCallbackSet(callbackSet){
		const listBindings = getList();
		for(var i=0, binding; binding=listBindings[i]; i++){
			if(binding.getCallbackSet()==callbackSet){
				return binding;
			}
		}
	}
}