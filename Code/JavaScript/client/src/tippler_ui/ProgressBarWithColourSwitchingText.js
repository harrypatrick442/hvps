import enableBindingToOthers from '../mvvm/enableBindingToOthers';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import E from '../ui_core/E';
import isNotNullOrUndefined from '../core/isNotNullOrUndefined';
import FrequencyLimitedCallback from '../core/FrequencyLimitedCallback';
import ResizeWatchers from '../ui_core/ResizeWatchers';
import ProgressBar from './ProgressBar';
const {throwNotProvided}=ParameterErrors;
export default class ProgressBarWithColourSwitchingText extends ProgressBar{
	constructor(props){
		super(props);
		const {propertyNameText, model} = props;
		this.resize = this.resize.bind(this);
		this._textChanged = this._textChanged.bind(this);
		this._textElement = E.div('text');
		this._barTextElement = E.div('text');
		this._innerElement.insertBefore(this._textElement, this._barElement);
		this._barElement.appendChild(this._barTextElement);
		this._propertyBindingText = PropertyBindingFactory.standard(this, model, propertyNameText, this._textChanged);
		this._resizeWatcher = ResizeWatchers.add({element:this._element, onResized:this.resize, staggered:false});
		setTimeout(this.resize, 0);
	}
	resize(){
		const width = `${this._element.offsetWidth}px`;
		this._barTextElement.style.width = width;
		this._textElement.style.width = width;
	}
	_textChanged(value){
		const text = isNotNullOrUndefined(value)?value:'';
		this._textElement.innerHTML = text;
		this._barTextElement.innerHTML = text;
	}
}
