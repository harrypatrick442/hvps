import ClickedOffHandle from './ClickedOffHandle';
import getAbsolute from './getAbsolute';
import EventNormalizer from '../core/EventNormalizer';
import Once from '../core/Once';
export default class ClickedOff {
	constructor(props){
		this._clicked = this._clicked.bind(this);
		this._onceClicked = new Once(this._clicked);
	}
    static register (element, callbackHide) {
		var handle =  new ClickedOffHandle(element, callbackHide, ClickedOff._dispose);
		setTimeout(function () {
			if (!ClickedOff._containsElement(element)) {
				if(handle.getDisposed())return;
				ClickedOff._handles.push(handle);
			}
		},0);
        return handle;
    };
	static remove(element){
		const handles =  ClickedOff._handles;
		var i=0;
		while(i<handles.length){
			const handle = handles[i];
			if(handle.getElement()===element)
			{
				handle.dispose();
				break;
			}
			i++;
		}
	}
	static _dispose(handle){
		var index = ClickedOff._handles.indexOf(handle);
		if(index<0)return;
		ClickedOff._handles.splice(index, 1);
	}
	static _touchedDocument(e){
		ClickedOff._onceClicked.trigger(e);
	}
    static _clickedDocument(e) {
		ClickedOff._onceClicked.trigger(e);
    }
	static triggerClicked(e){
		ClickedOff._clicked(e);
	}
	static _clicked(e){
        var x = e.x;
        var y = e.y;
		var handlesSnashot = ClickedOff._handles.slice();
        for (var i = 0, handle; handle = handlesSnashot[i]; i++) {
			var elements = handle.getElements();
			var withinOne=false;
			for(var j=0,element; element=elements[j];j++){
				var absolutePosition = getAbsolute(element);
				var xLeft = absolutePosition.left;
				var xRight = xLeft + element.offsetWidth;
				var yTop = absolutePosition.top;
				var yBottom = yTop + element.offsetHeight;
				if (!(x < xLeft || x > xRight || y < yTop || y > yBottom)) {
					withinOne=true;
					break;
				}
			}
			if(!withinOne)
				handle.hide();
        }
	}
    static _containsElement(element) {
        for (var i = 0, handle; handle = ClickedOff._handles[i]; i++) {
            if (handle.getElement() == element){
                return true;
			}
        }
        return false;
    }

}
ClickedOff._onceClicked = new Once(ClickedOff._clicked);
ClickedOff._handles=[];
document.addEventListener('mousedown', EventNormalizer.addFriendlyXYToMouse_LexicalClosure(ClickedOff._clickedDocument));
document.addEventListener('touchstart', EventNormalizer.addFriendlyXYToTouch_LexicalClosure(ClickedOff._touchedDocument));