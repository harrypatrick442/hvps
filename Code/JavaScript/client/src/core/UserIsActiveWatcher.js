import eventEnable from './eventEnable';
export default new (function UserIsActiveWatcher(){
	const self = this;
	eventEnable(self);
	let isActive = true;
	let becameActive;
	becameActive = ()=>{
		document.documentElement.removeEventListener('mousedown', becameActive);
		isActive = true;
			console.log('isActive was set true');
			console.log('DISPATCHING TRUE');
		self.dispatchEvent({type:'isActive', isActive:true});
	};
	window.document.addEventListener("visibilitychange", function() {
		const isVisible = document.visibilityState==='visible';
		if(isActive&&!isVisible){
			console.log('isActive was set false');
			isActive = false;
			document.documentElement.addEventListener('mousedown', becameActive);
			console.log('DISPATCHING FALSE');
			self.dispatchEvent({type:'isActive', isActive:false});
		}
	}); 
	Object.defineProperty(self, "isActive", {		
		get (){
			return isActive;
		}
	});
})();