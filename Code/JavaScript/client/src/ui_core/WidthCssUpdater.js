import ResizeWatcher from '../ui_core/ResizeWatcher';
import isNullOrUndefined from '../core/isNullOrUndefined';
import  './WidthCssUpdater.css';
export default class WidthCssUpdater{
	constructor({element, disposes, minWidths, widthProfile}){
		if(widthProfile==='standard'){
			if(isNullOrUndefined(minWidths)){
				minWidths=[600,900,1200];
			}
		}
		minWidths.sort(w=>w);
		if(widthProfile){
			element.classList.add('width-css-updater-'+widthProfile);
		}
		this._minWidths = minWidths;
		this._element = element;
		this.update = this.update.bind(this);
		this._getI = this._getI.bind(this);
		this._resizeWatcher = new ResizeWatcher({element, onResized:this.update});
		disposes.push(this._resizeWatcher.dispose);
	}
	update(){
		const i = this._getI();
		const cssClass = `width-${i}`;
		if(this._currentCssClass===cssClass)
			return;
		if(this._currentCssClass)
			this._element.classList.remove(this._currentCssClass);
		this._currentCssClass = cssClass;
		this._element.classList.add(cssClass);
	}
	_getI(){
		var i=0;
		const length = this._minWidths.length;
		while(i<length){
			const minWidth = this._minWidths[i];
			if(this._element.offsetWidth<minWidth){
				return i;
			}
			i++;
		}
		return i;
	}
}