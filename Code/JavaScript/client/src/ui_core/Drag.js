import EfficientMoveCycle from './EfficientMoveCycle';
import isNullOrUndefined from '../core/isNullOrUndefined';
function Drag({element, handleElement, getBoundsRectangle ,
	callbackDragged, callbackStarted}) {
    var self = this;
    var state = 0, start, minX, maxX, minY, maxY;
    const efficientMovingCycle = new EfficientMoveCycle({ 
		element:handleElement, stopPropagation:false, preventDefault:true, onStart,
		onMove, onEnd , friendlyXY:true});
	
	let startedButNotMovedYet= false;
    this.setPosition = function (array) {
        self.drag(array[0], array[1]);
    };
    this.getPosition = function () {
        var array = [];
        array[0] = element.offsetLeft;
        array[1] = element.offsetTop;
        return array;
    };
	this.updateBounds = function(){
		const boundsRectangle = getBoundsRectangle();
		minX = boundsRectangle.x;
		maxX = boundsRectangle.x+boundsRectangle.w;
		minY = boundsRectangle.y;
		maxY = boundsRectangle.y+boundsRectangle.h;
	};
    function onStart(e)
    {
		startedButNotMovedYet = true;
		const {x, y}= e;
        if (element.style.display === "none")
        {
            return;
        }
        start = [element.offsetLeft - x, element.offsetTop - y];
        self.updateBounds();
        state = 1;
    }
    function onMove(e)
    {
		if(startedButNotMovedYet){
			startedButNotMovedYet = false;
			if(!Drag.cancel)
				if(callbackStarted)callbackStarted(e);
		}
		const {x, y}= e;
        if(!Drag.cancel)
        if (state == 1) {
            self.drag((start[0] + x), (start[1] + y));
            callbackDragged&&callbackDragged();
        }
    }
    this.drag=function(x, y)
    {
		const overlappingLeft = x < minX;
		const maxXMinusWidth = maxX-element.offsetWidth;
		const overlappingRight = x > maxXMinusWidth;
		if (overlappingLeft) {
			if(!overlappingRight)
				x = minX;
		}
		else {
			if (overlappingRight) {
				x = maxXMinusWidth;
			}
		}
		element.style.left = String(x) + 'px';
		
		const overlappingTop = y < minY;
		const maxYMinusHeight = maxY-element.offsetHeight;
		const overlappingBottom = y> maxYMinusHeight ;
		if (overlappingTop) {
			if (!overlappingBottom)
				y = minY;
		}
		else {
			if (overlappingBottom) {
				y = maxYMinusHeight;
			}
		}
		element.style.top = String(y) + 'px';
    };
    function onEnd(e) {         
        Drag.cancel=false;
        state=0;
    };
}
export default Drag;