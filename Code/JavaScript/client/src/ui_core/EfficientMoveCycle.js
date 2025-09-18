import ParameterErrors from '../errors/ParameterErrors';
import EventNormalizer from '../core/EventNormalizer';
import Once from '../core/Once';
import HandledManager from '../core/HandledManager';
const { throwNotProvided, throwNotOfType} = ParameterErrors;
export default class EfficientMoveCycle{
	constructor(props){
		const { element, /*stopPropagation,*/ preventDefault, onStart, onMove, onEnd , friendlyXY} = props;
		if (!element) throwNotProvided("element");
		this._element = element;
		/*this._stopPropagation = (stopPropagation === undefined || stopPropagation === null)
			? false : stopPropagation;*/
		this._preventDefault = (preventDefault === undefined || preventDefault === null)
			? false : preventDefault;

		if (!onStart) throwNotProvided("onStart");
		if (typeof (onStart) !== "function") throwNotOfType("onStart", "function");
		this._onStartMouse = friendlyXY?EventNormalizer.addFriendlyXYToMouse_LexicalClosure(onStart):onStart;
		this._onStartTouch = friendlyXY?EventNormalizer.addFriendlyXYToTouch_LexicalClosure(onStart):onStart;
		
		if (!onMove) throwNotProvided("onMove");
		if (typeof (onMove) !== "function") throwNotOfType("onMove", "function");
		this._onMoveMouse = friendlyXY?EventNormalizer.addFriendlyXYToMouse_LexicalClosure(onMove):onMove;
		this._onMoveTouch = friendlyXY?EventNormalizer.addFriendlyXYToTouch_LexicalClosure(onMove):onMove;

		if (!onEnd) throwNotProvided("onEnd");
		if (typeof (onEnd) !== "function") throwNotOfType("onEnd", "function");
		this._onEndOnce = new Once(onEnd);
		this._onEndMouse = friendlyXY?EventNormalizer.addFriendlyXYToMouse_LexicalClosure(this._onEndOnce.trigger):this._onEndOnce.trigger;
		this._onEndTouch = friendlyXY?EventNormalizer.addFriendlyXYToTouch_LexicalClosure(this._onEndOnce.trigger):this._onEndOnce.trigger;

		this._mouseDown = this._mouseDown.bind(this);
		this._mouseMove = this._mouseMove.bind(this);
		this._mouseUp = this._mouseUp.bind(this);
		this._stopPropagationIfRequired = this._stopPropagationIfRequired.bind(this);
		this._preventDefaultIfRequired = this._preventDefaultIfRequired.bind(this);
		this._addMouseMoveEvent = this._addMouseMoveEvent.bind(this);
		this._addMouseUpEvent = this._addMouseUpEvent.bind(this);
		
		this._touchStart = this._touchStart.bind(this);
		//this._touchStartAnywhere = this._touchStartAnywhere.bind(this);
		this._touchMove = this._touchMove.bind(this);
		this._touchEnd = this._touchEnd.bind(this);
		this._addTouchEnd = this._addTouchEnd.bind(this);
		this._addTouchMove = this._addTouchMove.bind(this);
		element.addEventListener('mousedown', this._mouseDown, {passive: false});
		element.addEventListener('touchstart', this._touchStart, {passive: false});
	}
	_mouseDown(e) {
		if (!e) e = window.event;
		if(HandledManager.handled(e))return;
		if(this._friendlyXY)this._addFriendlyMouseXY(e);
		if (this._onStartMouse(e, false) == false) return;
		EfficientMoveCycle._clearCurrentMouseMove();
		EfficientMoveCycle._clearCurrentMouseUp()
		this._addMouseMoveEvent();
		this._addMouseUpEvent();
		this._stopPropagationIfRequired(e);
		this._preventDefaultIfRequired(e);
	}
	_mouseMove(e) {
		if (!e) e = window.event;
		if(HandledManager.handled(e))return;
		this._onMoveMouse(e, false);
		this._stopPropagationIfRequired(e);
		this._preventDefaultIfRequired(e);
	}
	_mouseUp(e) {
		if (!e) e = window.event;
		if(HandledManager.handled(e))return;
		this._onEndMouse(e, false);
		EfficientMoveCycle._clearCurrentMouseUp();
		EfficientMoveCycle._clearCurrentMouseMove();
		this._stopPropagationIfRequired(e);
		this._preventDefaultIfRequired(e);
	}
	
	
	
	_touchStart(e) {
		if (!e) e = window.event;
		if(HandledManager.handled(e))return;
		if (this._onStartTouch(e, true) == false) return;
		EfficientMoveCycle._clearCurrentTouchMove();
		EfficientMoveCycle._clearCurrentTouchEnd();
		this._addTouchMove();
		this._addTouchEnd();
	}
	/*_touchStartAnywhere(e){
		if (!e)e = window.event;
		self[S.ON_START_ANYWHERE](e, true);
	}*/
	_touchMove(e) {
		if (!e) e = window.event;
		if(HandledManager.handled(e))return;
		this._onMoveTouch(e, true);
		this._stopPropagationIfRequired(e);
		this._preventDefaultIfRequired(e);
	}
	_touchEnd(e) {
		if (!e) e = window.event;
		if(HandledManager.handled(e))return;
		const keep = this._onEndTouch(e, true);
		if(keep)return;
		EfficientMoveCycle._clearCurrentTouchEnd();
		EfficientMoveCycle._clearCurrentTouchMove();
	}
	_addTouchMove(){
		document.documentElement.addEventListener('touchmove', this._touchMove, {passive: false});
		EfficientMoveCycle._currentTouchMove = this._touchMove;
	}
	_addTouchEnd(){
		document.documentElement.addEventListener('touchend', this._touchEnd, {passive: false});
		EfficientMoveCycle._currentTouchEnd = this._touchEnd;
	}
	
	_stopPropagationIfRequired(e) {
		if (this._stopPropagation) {
			e.stopPropagation && e.stopPropagation();
		}
	}
	_preventDefaultIfRequired(e) {
		if (this._preventDefault) {
			e.preventDefault && e.preventDefault();
		}
	}
	_addMouseMoveEvent() {
		document.documentElement.addEventListener('mousemove', this._mouseMove, {passive: false});
		EfficientMoveCycle._currentMouseMove = this._mouseMove;
	}
	_addMouseUpEvent() {
		document.documentElement.addEventListener('mouseup', this._mouseUp, {passive: false});
		EfficientMoveCycle._currentMouseUp = this._mouseUp;
	}	
	dispose(){
		this._element.removeEventListener('mousedown', this._mouseDown);
		this._element.removeEventListener('touchstart', this._touchStart);
	}
	static _clearCurrentMouseUp() {
		if (!EfficientMoveCycle._currentMouseUp) return;
		document.documentElement.removeEventListener('mouseup', EfficientMoveCycle._currentMouseUp);
		EfficientMoveCycle._currentMouseUp = undefined;
	}
	static _clearCurrentMouseMove() {
		if (!EfficientMoveCycle._currentMouseMove) return;
		document.documentElement.removeEventListener('mousemove', EfficientMoveCycle._currentMouseMove);
		EfficientMoveCycle._currentMouseMove = undefined;
	}
	
	
	static _clearCurrentTouchEnd(){
		if(!EfficientMoveCycle._currentTouchEnd)return;
		document.documentElement.removeEventListener('touchend', EfficientMoveCycle._currentTouchEnd);
		EfficientMoveCycle._currentTouchEnd=undefined;
	}
	static _clearCurrentTouchMove(){
		if(!EfficientMoveCycle._currentTouchMove)return;
		document.documentElement.removeEventListener('touchmove', EfficientMoveCycle._currentTouchMove);
		EfficientMoveCycle._currentTouchMove=undefined;
	}	
}
