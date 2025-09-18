export default class OnlineStatusSource{
	constructor(){
		eventEnable(this);
		if(isNullOrUndefined(window.navigator.onLine))
			return;
		
	}
}