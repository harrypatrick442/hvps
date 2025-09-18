import E from '../ui_core/E';
import ResizeWatchers from '../ui_core/ResizeWatchers';
import ScrollBar from './ScrollBar';
import './Scroller.scss';
export default class Scroller{
	constructor({horizontalElseVertical, childElement}){
		//The solution is for the outside element to have a desirable height set and capped.
		//Then the scrollbar will size its height correctly.
		this._element = E.div('scroller');
		childElement.classList.add('scroller-child');
		this._scrollBar = new ScrollBar({
			horizontalElseVertical:false, 
			elementBeingScrolled:childElement,
			positionedRelative:false
		});
		this._element.appendChild(childElement);
		this._element.appendChild(this._scrollBar.element);
		this.resize = this._scrollBar.resize;
		this._resizeWatcher = ResizeWatchers.add({
			element:childElement, 
			onResized:()=>{
				setTimeout(this._scrollBar.resize, 0);
			},
			staggered:false});
	}
	get element(){
		return this._element;
	}
	dispose(){
		this._resizeWatcher.dispose();
	}
}