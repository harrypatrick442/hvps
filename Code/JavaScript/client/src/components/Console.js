import E from '../ui_core/E';
import isNullOrUndefined from '../core/isNullOrUndefined';
import ConsoleMessageType from '../enums/ConsoleMessageType';
import './Console.scss';
export default class Console{	
	constructor({
			model, 
			eventNameAppendLine, 
			eventNameClear,
			maxNLines
		}){
		this._model = model;
		console.log(model);
		if(isNullOrUndefined(maxNLines))
			throw new Error('maxNLines');
		this._maxNLines = maxNLines;
		const disposes = [];
		this._disposes = disposes;
		this._lines = [];
		this._handleAppendLine = this._handleAppendLine.bind(this);
		this._handleClear = this._handleClear.bind(this);
		this.dispose = this.dispose.bind(this);
		this._element = E.div('console');
		if(!isNullOrUndefined(eventNameAppendLine)){
			disposes.push(model.addEventListener(eventNameAppendLine, this._handleAppendLine));
		}
		if(!isNullOrUndefined(eventNameClear)){
			disposes.push(model.addEventListener(eventNameClear, this._clear));
		}
	}
	get element(){
		return this._element;
	}
	_handleAppendLine({str, consoleMessageType}){
		console.log('handleAppendLine');
		const element = E.div('line');
		element.textContent = str;
		if(isNullOrUndefined(consoleMessageType)){
			consoleMessageType = ConsoleMessageType.Info;
		}
		element.classList.add(ConsoleMessageType.getCssClass(consoleMessageType));
		this._lines.push(element);
		this._element.appendChild(element);
		while(this._lines.length>this._maxNLines){
			this._element.removeChild(this._lines.splice(0, 1)[0]);
		}
	}
	_handleClear(){
		this._lines.forEach(l=>this._element.removeChild(l));
	}
	dispose(){
		this._disposes.forEach(d=>d());
	}
}