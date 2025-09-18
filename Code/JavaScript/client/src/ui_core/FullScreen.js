import eventEnable from '../core/eventEnable';
class FullScreen{
	static toggle(element){
		if(document.fullscreenElement)
			FullScreen.exit();
		else
			FullScreen.enable(element);
	}
	static get isSupported(){
		return document.fullscreenEnabled;
	}
	static get isEnabled(){
		return document.fullscreenElement?true:false;
	}
	static enable(element){
		if(FullScreen.isEnabled){
			if(FullScreen._element===element)return;
			FullScreen.exit();
		}
		if (element.requestFullscreen) {
			element.requestFullscreen();
		} else if (element.webkitRequestFullscreen) { /* Safari */
			element.webkitRequestFullscreen();
		} else if (element.msRequestFullscreen) { /* IE11 */
			element.msRequestFullscreen();
		}
		FullScreen._dispatchChange();
	}
	static exit(){
		if(!FullScreen.isEnabled)return;
		document.exitFullscreen();
		FullScreen._dispatchChange();
	}
	static _dispatchChange(){
		
	}
}
eventEnable(FullScreen);
export default FullScreen;