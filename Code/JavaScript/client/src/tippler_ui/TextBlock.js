
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
export default class TextBlock{
	constructor(props){
		const {className, classNames, propertyName, methodNameClick, model, text}=props;
		if(!model&&!text) throw new Error('No model provided');
		this._valueChanged = this._valueChanged.bind(this);
		this.dispose = this.dispose.bind(this);
		this._element = document.createElement('div');
		this._element.classList.add('text-block');
		if (className)
			this._element.classList.add(className);
		if (classNames) 
			classNames.forEach((className)=>{
				this._element.classList.add(className);
			});
		if(methodNameClick){
			this._element.addEventListener('click', function(){model[methodNameClick]();});
		}
		if(propertyName){
			this._propertyBinding = PropertyBindingFactory.standard(this, model, propertyName, this._valueChanged);
		}
		else{
			this._valueChanged(text);
		}
	}
	get element(){
		return this._element;
	}
	dispose(){
		this.myBindings&&this.myBindings.dispose();
	}
	_valueChanged(value){
		this._element.textContent=value!=undefined&&value!=null?String(value):'';
	}
};