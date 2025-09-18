import _createTextButton from '../ui_core/_createTextButton';
import E from '../ui_core/E';
import ConsoleOverloads from '../core/ConsoleOverloads';
import _parseErrorToString from '../core/_parseErrorToString';
import BrowserHelper from '../browser/BrowserHelper';
import './DebugMenu.css';
const TEXT_SHOW='Console', TEXT_HIDE='Hide';
export default class DebugMenu{
    constructor() {
		const disposes = [];
		this._disposes = disposes;
		this._handleError = this._handleError.bind(this);
		this._handleLog = this._handleLog.bind(this);
		this._toggleShowHide = this._toggleShowHide.bind(this);
		this._copyToClipboardOnLongLick = this._copyToClipboardOnLongLick.bind(this);
		this.diaspose = this.diaspose.bind(this);
		this._showing = false;
		this._element = E.div('debug-menu');
		this._buttonToggleShowHide = _createTextButton({className:'button-toggle-show-hide', callback:this._toggleShowHide, disposes, useMouseDown:false, text:TEXT_SHOW});
		this._element.appendChild(this._buttonToggleShowHide);
		this._consoleElement = E.div('console');
		this._element.appendChild(this._consoleElement);
		document.body.appendChild(this._element);
		ConsoleOverloads.overloadLog(this._handleLog);
		ConsoleOverloads.overloadError(this._handleError);
	}
	_handleError(e){
		const errorElement = E.div('error');
		errorElement.innerHTML = _parseErrorToString(e);
		this._consoleElement.appendChild(errorElement);
		this._copyToClipboardOnLongLick(errorElement);
	}
	_handleLog(e){
		const logElement = E.div('log');
		logElement.innerHTML = String(e);
		this._consoleElement.appendChild(logElement);
		this._copyToClipboardOnLongLick(logElement);
	}
	_toggleShowHide(){
		this._showing =! this._showing;
		if(this._showing){
			this._buttonToggleShowHide.innerHTML = TEXT_HIDE;
			this._element.classList.add('showing');
		}
		else{
			this._buttonToggleShowHide.innerHTML = TEXT_SHOW;
			this._element.classList.remove('showing');
		}
	}
	_copyToClipboardOnLongLick(element){
		if(!navigator.clipboard)return;
		const handle = ()=>navigator.clipboard.writeText(element.innerHTML);
		element.addEventListener('click', handle);
		this._disposes.push(()=>element.removeEventListener('longclick', handle));
	}
	diaspose(){
		this._disposes.forEach(d=>d());
	}
}