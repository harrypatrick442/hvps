import ClickedOff from './ClickedOff';
import ParameterErrors from '../errors/ParameterErrors';
import isNullOrUndefined from '../core/isNullOrUndefined';
const {throwNotProvided}=ParameterErrors;
export default class ClickedOffManager{
	constructor(props){
		const {elementRef, element, hide, additionalElements}=props;
		if((!elementRef)&&(!element))throwNotProvided('elementRef and element');
		if(!hide)throwNotProvided('hide');
		this._elementRef = elementRef;
		this._element = element;
		this._hide = hide;
		this._additionalElements = additionalElements;
		this.dispose = this.dispose.bind(this);
		this.register = this.register.bind(this);
		this._clickedOffHandle=null;
	}
	register(){
		this.dispose();
		this._clickedOffHandle  = ClickedOff.register(this._elementRef?
			this._elementRef.current:this._element,this._hide);
		if(!isNullOrUndefined(this._additionalElements))
			this._additionalElements.forEach(a=>this._clickedOffHandle.addAdditionalElement(a));
		return this._clickedOffHandle;
	}
	dispose(){
		this._clickedOffHandle&&this._clickedOffHandle.dispose();
		this._clickedOffHandle=null;
	}
}