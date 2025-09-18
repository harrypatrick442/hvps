import ClickedOffManager from '../ui_core/ClickedOffManager';
export default class Popup{
	constructor(props){
		const {element}=props;
		if(!element) throw new Error('element');
		this._element = element;
		this._element.classList.add('popup');
		this.hide = this.hide.bind(this);
		this.show = this.show.bind(this);
		this.setVisible = this.setVisible.bind(this);
		this._clickedOffManager = new ClickedOffManager({ element, hide:this.hide});
	}
	get element(){
		return this._element;
	}
	hide(){
		this.setVisible(false);
		this._clickedOffManager.dispose();
	}
	show(){
		this.setVisible(true);
		this._clickedOffManager.register();
	}
	setVisible(value){
		if(value)this._element.classList.add('visible');
		else this._element.classList.remove('visible');
	}
}