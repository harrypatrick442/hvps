import ClickedOffManager from '../ui_core/ClickedOffManager';
import PropertyBindingFactory from '../mvvm/PropertyBindingFactory';
export default class Popup{
	constructor(props){
		const {element, model, propertyNameVisible}=props;
		if(!element) throw new Error('element');
		this._element = element;
		this._element.classList.add('popup');
		this.hide = this.hide.bind(this);
		this.show = this.show.bind(this);
		this._visibleChanged = this._visibleChanged.bind(this);
		this._clickedOffManager = new ClickedOffManager({ element,
			hide:this.hide});
		this._propertyBindingVisible = PropertyBindingFactory
			.standard(this, model, propertyNameVisible, this._visibleChanged);
	}
	get element(){
		return this._element;
	}
	hide(){
		this._propertyBindingVisible.set(false);
	}
	show(){
		this._propertyBindingVisible.set(true);
	}
	_visibleChanged(value){
		if(value){
			this._element.classList.add('visible');
			this._clickedOffManager.register();
			return;
		}
		this._element.classList.remove('visible');
		this._clickedOffManager.dispose();
	}
}