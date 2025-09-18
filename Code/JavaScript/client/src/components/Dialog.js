import i from '../ui_core/i';
import E from '../ui_core/E';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import DialogButton from './DialogButton';
import ClickedOffManager from '../ui_core/ClickedOffManager';
import './Dialog.css';
export default class Dialog{
	constructor({preventInterraction, options, message, title,
			className, onClickClose, onClickOff, disposeElseHideOnClose, callbackDontShowAgain, 
			closeOnClickedOff}){
		this._disposeElseHideOnClose=disposeElseHideOnClose!==false;
		this._preventInterraction=(preventInterraction===undefined
			||preventInterraction===null)?true:preventInterraction;
		this._callbackClickClose = onClickClose;
		this._onClickOff = onClickOff;
		const disposes = [];
		this._disposes = disposes;
		this.hide = this.hide.bind(this);
		this.show = this.show.bind(this);
		this.dispose = this.dispose.bind(this);
		this._setVisible = this._setVisible.bind(this);
		this._handleClickButtonClose = this._handleClickButtonClose.bind(this);
		
		this._element = E.div('dialog');
		if(className)
			this._element.classList.add(className);
		this._innerElement = E.div('inner');
		this._element.appendChild(this._innerElement);
		this._topElement = E.div('top');
		this._innerElement.appendChild(this._topElement);
		this._titleElement = E.div('title');
		this._topElement.appendChild(this._titleElement);
		this._buttonClose = _createImageHoverButton(i('Close'), i('CloseHover'), 'close-button', 
			this._handleClickButtonClose, disposes);
		this._topElement.appendChild(this._buttonClose);
		this._messageElement = E.div('message');
		this._innerElement.appendChild(this._messageElement);
		this._buttonsElement = E.div('buttons');
		this._innerElement.appendChild(this._buttonsElement);
		this.callbackDontShowAgain = callbackDontShowAgain;
		this._dontShowAgainElement = E.div('dont-show-again');
		this._dontShowAgainElement.textContent="Don't show again";
		this._checkboxDontShowAgain = E.checkbox();
		this._dontShowAgainElement.appendChild(this._checkboxDontShowAgain);
		this._innerElement.appendChild(this._dontShowAgainElement);
		if(closeOnClickedOff)
			this._clickedOffManager = new ClickedOffManager({
				element:this._element, hide:()=>{
					if(this._onClickOff)
						this._onClickOff();
					this.hide();
				}});
		this.title = title;
		this.message = message;
		this.options = options;
	}
	set title(value){
		this._titleElement.textContent = value?value:'';
	}
	set message(value){
		this._messageElement.textContent = value;
	}
	set options(value){
		if(this._buttons)
			this._buttons.forEach(b=>b.dispose());
		if(!value)return;
		if(!Array.isArray(value))
			value=[value];
		this._buttons = value.map(({text, callback, className})=>{
			const dialogButton = new DialogButton({
				text,
				onClick:this._createOnClickForOption(callback),
				className, 
				disposes:this._disposes
			});
			this._buttonsElement.appendChild(dialogButton.element);
			return dialogButton;
		});
	}
	set callbackDontShowAgain(value){
		if(value)
			this._element.classList.add('has-dont-show-again');
		else
			this._element.classList.remove('has-dont-show-again');
		this._callbackDontShowAgain = value;
	}
	get element(){
		return this._element;
	}
	
	show(params){
		this._clickedOffManager?.register();
		this._checkboxDontShowAgain.checked = false;
		if(params){
			const {options, message, title, type, onClickClose, 
				onClickOff, callbackDontShowAgain} = params;
			this._onClickOff = onClickOff;
			if(this._currentDialogType)
				this._element.classList.remove(this._currentDialogType);
			this._currentDialogType = type;
			if(type){
				this._element.classList.add(type);
			}
			this.options = options;
			this.message = message;
			this.title = title;
			this._callbackClickClose = onClickClose;
			this.callbackDontShowAgain = callbackDontShowAgain;
		}
		this._setVisible(true);
		return this;
	}
	hide(){
		this._clickedOffManager?.dispose();
		this._setVisible(false);
	}
	/*hide(){
		if(this._disposeOnHide){
			this.dispose();
			return;
		}
		this.setState({visible:false});
	}*/
	_handleClickButtonClose(){
		this._callbackClickClose&&this._callbackClickClose();
		if(this._checkboxDontShowAgain.checked)
			this._callbackDontShowAgain&&this._callbackDontShowAgain();
		if(this._disposeElseHideOnClose)
			this.dispose();
		else
			this.hide();
	}
	_createOnClickForOption(callback){
		return ()=>{
			callback&&callback();
			if(this._checkboxDontShowAgain.checked)
				this._callbackDontShowAgain&&this._callbackDontShowAgain();
			if(this._disposeElseHideOnClose)
				this.dispose();
			else
				this.hide();
		};
	}
	dispose(){
		this._clickedOffManager?.dispose();
		if(this._element.parentNode)
			this._element.parentNode.removeChild(this._element);
		this._disposes.forEach(d=>d());
	}
	_setVisible(value){
		if(value)
			this._element.classList.add('visible');
		else
			this._element.classList.remove('visible');
	}
}