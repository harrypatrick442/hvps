import isMobile from '../core/isMobile';
import HandledManager from '../core/HandledManager';
import Timer from '../core/Timer';
function Resizer({element, minWidth, minHeight, maxWidth, maxHeight,
		getBoundsRectangle, callbackResized, callbackInstantaneous, callbackStarted, 
		callbackFinished,
		toInclude, fixedAspectRatio}) {
    var bounds;
	if(toInclude===undefined||toInclude===null)toInclude = {top:true, left:true, bottom:true, right:true,
		leftTop:true, leftBottom:true, rightTop:true, rightBottom:true};
		
	var divLeft, divTop , divRight, divBottom, divLeftTop, divLeftBottom, divRightTop, divRightBottom;
	
	this.setFixedAspectRatio = function(value){
		fixedAspectRatio= value;
	};
	var minusPaddingString = '-'+Resizer.padding+'px';
	if(toInclude.leftTop){
		divLeftTop = document.createElement('div');
		setCornerGeneric(divLeftTop);
		divLeftTop.style.top = minusPaddingString;
		divLeftTop.style.left = minusPaddingString;
		divLeftTop.style.cursor = 'nw-resize';
	}
	if(toInclude.leftBottom){
		divLeftBottom = document.createElement('div');
		setCornerGeneric(divLeftBottom);
		divLeftBottom.style.bottom = minusPaddingString;
		divLeftBottom.style.left = minusPaddingString;
		divLeftBottom.style.cursor = 'sw-resize';
	}
	
	if(toInclude.rightTop){
		divRightTop = document.createElement('div');
		setCornerGeneric(divRightTop);
		divRightTop.style.top = minusPaddingString;
		divRightTop.style.right = minusPaddingString;
		divRightTop.style.cursor = 'sw-resize';
	}
	
	if(toInclude.rightBottom){
		divRightBottom = document.createElement('div');
		setCornerGeneric(divRightBottom);
		divRightBottom.style.bottom = minusPaddingString;
		divRightBottom.style.right = minusPaddingString;
		divRightBottom.style.cursor = 'nw-resize';
	}
	if(toInclude.left){
		divLeft = document.createElement('div');
		divLeft.style = `position:absolute;z-index:100;width:${Resizer.WIDTH}px;height:100%;left:-${Resizer.padding}px;top:0px;cursor:e-resize;`;
	}
	if(toInclude.right){
		divRight = document.createElement('div');
		divRight.style = `position:absolute;z-index:100;width:${Resizer.WIDTH}px;height:100%;right:-${Resizer.padding}px;top:0px;cursor:e-resize;`;
    }
	if(toInclude.top){
		divTop = document.createElement('div');
		divTop.style = `position:absolute;z-index:100;width:100%;height:${Resizer.WIDTH}px;top:-${Resizer.padding}px;left:0px;cursor:n-resize;`;
    }
	if(toInclude.bottom){
		divBottom = document.createElement('div');
		divBottom.style = `position:absolute;z-index:100;width:100%;height:${Resizer.WIDTH}px;bottom:-${Resizer.padding}px;left:0px;cursor:n-resize;`;
    }
	function setCornerGeneric(element)
    {
        element.style.position = 'absolute';
        element.style.width = '10px';
        element.style.height = '10px';
        element.style.zIndex = '102';
    }
    if(minWidth<maxWidth)
    {
		if(divLeft)
			element.appendChild(divLeft);
		if(divRight)
			element.appendChild(divRight);
    if(minHeight<maxHeight)
    {
		if(toInclude.leftTop)
			element.appendChild(divLeftTop);
		if(toInclude.leftBottom)
			element.appendChild(divLeftBottom);
		if(toInclude.rightTop)
			element.appendChild(divRightTop);
		if(toInclude.rightBottom)
			element.appendChild(divRightBottom);
    }
    }
    if(minHeight<maxHeight)
    {
		if(toInclude.top)
			element.appendChild(divTop);
		if(toInclude.bottom)
			element.appendChild(divBottom);
    }
    if(minWidth<maxWidth&&minHeight<maxHeight)
    {
		if(toInclude.leftTop)
			element.appendChild(divLeftTop);
		if(toInclude.leftBottom)
			element.appendChild(divLeftBottom);
		if(toInclude.rigtTop)
			element.appendChild(divRightTop);
		if(toInclude.rightButton)
			element.appendChild(divRightBottom);
    }
    var state = 'up';
    var startOffset = [];
    var timer;
    var moveEvent;
    var upEvent;
    this.setDimensions = function (array) {
        element.style.width = String(array[0]) + 'px';
        element.style.height = String(array[1]) + 'px';
    };
    this.getDimensions = function () {
        var array = [];
        array[0] = element.offsetWidth;
        array[1] = element.offsetHeight;
        return array;
    };
    function onMouseDown(e)
    {
		updateBounds();
		if(!HandledManager.handled(e)){
			e.preventDefault&&e.preventDefault();
			if (!isMobile)
			{
				moveEvent = function (e) {
					if (!e)
						e = window.event;
					//e.stopPropagation&&e.stopPropagation();
					e.preventDefault&&e.preventDefault();
					doResize(e);
					
				};
				document.documentElement.addEventListener("mousemove", moveEvent);
				upEvent = function (e)
				{
					if (!e)
						var e = window.event;

					state = 'up';
					document.documentElement.removeEventListener("mousemove", moveEvent);
					document.documentElement.removeEventListener("mouseup", upEvent);
					callbackFinished&&callbackFinished();
				};
				document.documentElement.addEventListener("mouseup", upEvent);
			}
			else
			{
				moveEvent = function (e) {
					if (!e)
						var e = window.event;
					doResize(e);
				};
				document.documentElement.addEventListener("touchmove", moveEvent);
				upEvent = function (e)
				{
					if (!e)
						var e = window.event;

					state = 'up';
					document.documentElement.removeEventListener("touchmove", moveEvent);
					document.documentElement.removeEventListener("touchend", upEvent);
					callbackFinished&&callbackFinished();
				};
				document.documentElement.addEventListener("touchend", upEvent);
			}
			timer = new Timer({callback:function () {
				if (callbackResized != undefined) {
				try
				{
					callbackResized();
				}
				catch(ex)
				{
					console.error(ex);
				}
				}
			}, delay:1000, nTicks:1});
			callbackStarted&&callbackStarted(e);
		}
    }
    if (!isMobile)
    {
		if(divLeft)
			divLeft.onmousedown = function (e) {
				state = 'down-left';
				startOffset[0] = element.offsetWidth + e.pageX;
				startOffset[1] = element.offsetLeft - e.pageX;
				onMouseDown(e);
			};
		if(divRight)
			divRight.onmousedown = function (e) {
				state = 'down-right';
				startOffset[0] = element.offsetWidth - e.pageX;
				onMouseDown(e);
			};
		if(divTop)
			divTop.onmousedown = function (e) {
				state = 'down-top';
				startOffset[0] = element.offsetHeight + e.pageY ;
				startOffset[1] = element.offsetTop - e.pageY;
				onMouseDown(e);
			};
		if(divBottom)
			divBottom.onmousedown = function (e) {
				state = 'down-bottom';
				startOffset[0] = element.offsetHeight - e.pageY;
				onMouseDown(e);
			};
		if(divLeftTop)
			divLeftTop.onmousedown = function (e) {
				state = 'down-left-top';
				startOffset[0] = element.offsetWidth + e.pageX ;
				startOffset[1] = element.offsetLeft - e.pageX;
				startOffset[2] = element.offsetHeight + e.pageY ;
				startOffset[3] = element.offsetTop - e.pageY;
				onMouseDown(e);
			};
		if(divLeftBottom)
			divLeftBottom.onmousedown = function (e) {
				state = 'down-left-bottom';
				startOffset[0] = element.offsetWidth + e.pageX ;
				startOffset[1] = element.offsetLeft - e.pageX;
				startOffset[2] = element.offsetHeight - e.pageY;
				onMouseDown(e);
			};
		if(divRightTop)
			divRightTop.onmousedown = function (e) {
				state = 'down-right-top';
				startOffset[0] = element.offsetWidth - e.pageX;
				startOffset[1] = element.offsetHeight + e.pageY;
				startOffset[2] = element.offsetTop - e.pageY;
				onMouseDown(e);
			};
		if(divRightBottom)
			divRightBottom.onmousedown = function (e) {
				state = 'down-right-bottom';
				startOffset[0] = element.offsetWidth - e.pageX;
				startOffset[1] = element.offsetHeight - e.pageY;
				onMouseDown(e);
			};
    }
    else
    {
		if(divLeft)
			divLeft.touchstart = function (e) {
				state = 'down-left';
				startOffset[0] = element.offsetWidth + e.touches[0].pageX ;
				startOffset[1] = element.offsetLeft - e.touches[0].pageX;
				onMouseDown(e);
			};
		if(divRight)
			divRight.touchstart = function (e) {
				state = 'down-right';
				startOffset[0] = element.offsetWidth - e.touches[0].pageX;
				onMouseDown(e);
			};
		if(divTop)
			divTop.touchstart = function (e) {
				state = 'down-top';
				startOffset[0] = element.offsetHeight + e.touches[0].pageY ;
				startOffset[1] = element.offsetTop - e.touches[0].pageY;
				onMouseDown(e);
			};
		if(divBottom)
			divBottom.touchstart = function (e) {
				state = 'down-bottom';
				startOffset[0] = element.offsetHeight - e.touches[0].pageY;
				onMouseDown(e);
			};
		if(divLeftTop)
			divLeftTop.touchstart = function (e) {
				state = 'down-left-top';
				startOffset[0] = element.offsetWidth + e.touches[0].pageX ;
				startOffset[1] = element.offsetLeft - e.touches[0].pageX;
				startOffset[2] = element.offsetHeight + e.touches[0].pageY ;
				startOffset[3] = element.offsetTop - e.touches[0].pageY;
				onMouseDown(e);
			};
		if(divLeftBottom)
			divLeftBottom.touchstart = function (e) {
				state = 'down-left-bottom';
				startOffset[0] = element.offsetWidth + e.touches[0].pageX ;
				startOffset[1] = element.offsetLeft - e.touches[0].pageX;
				startOffset[2] = element.offsetHeight - e.touches[0].pageY;
				onMouseDown(e);
			};
		if(divRightTop)
			divRightTop.touchstart = function (e) {
				state = 'down-right-top';
				startOffset[0] = element.offsetWidth - e.touches[0].pageX;
				startOffset[1] = element.offsetHeight + e.touches[0].pageY;
				startOffset[2] = element.offsetTop - e.touches[0].pageY;
				onMouseDown(e);
			};
		if(divRightBottom)
			divRightBottom.touchstart = function (e) {
				state = 'down-right-bottom';
				startOffset[0] = element.offsetWidth - e.touches[0].pageX;
				startOffset[1] = element.offsetHeight - e.touches[0].pageY;
				onMouseDown(e);
			};
    }
	function updateBounds(){
		const boundsRectangle = getBoundsRectangle();
		bounds = {
			maxX:boundsRectangle.x+boundsRectangle.w,
			minX:boundsRectangle.x,
			minY:boundsRectangle.y,
			maxY:boundsRectangle.y+boundsRectangle.h
		};
	}
    function resizeLeft(x, y, a, b) {
        var width = a - x;
        var left = b + x;
        if (left < bounds.minX || width < minWidth || width > maxWidth)
        {
            return;
        }
        element.style.width = String(width) + 'px';
        element.style.left = String(left) + 'px';
    }
    function resizeRight(x, y, a) {
        var width = a + x;
        if (width < minWidth || width > maxWidth || width + element.offsetLeft > bounds.maxX) {
            return;
        }
        element.style.width = String(width) + 'px';
    }
    function resizeTop(x, y, a, b) {
        var height = a - y;
        var top = b + y;
        if (top < bounds.minY || height < minHeight || height > maxHeight) {
            return;
        }
        element.style.height = String(height) + 'px';
        element.style.top = String(top) + 'px';
    }
    function resizeBottom(x, y, a) {
        var height = a + y;
        if (height < minHeight || height > maxHeight || element.offsetTop + height > bounds.maxY) {
            return;
        }
        element.style.height = String(height) + 'px';
    }
    function doResize(e) {
        var x = e.pageX;
        var y = e.pageY;
		
        switch (state) {
            case "up":
                return;
            case "down-left":
                resizeLeft(x, y, startOffset[0], startOffset[1]);
                break;
            case "down-right":
                resizeRight(x, y, startOffset[0]);
                break;
            case "down-top":
                resizeTop(x, y, startOffset[0], startOffset[1]);
                break;
            case "down-bottom":
                resizeBottom(x, y, startOffset[0]);
                break;
            case "down-left-top":
                resizeLeft(x, y, startOffset[0], startOffset[1]);
                resizeTop(x, y, startOffset[2], startOffset[3]);
                break;
            case "down-left-bottom":
                resizeLeft(x, y, startOffset[0], startOffset[1]);
                resizeBottom(x, y, startOffset[2]);
                break;
            case "down-right-top":
                resizeRight(x, y, startOffset[0]);
                resizeTop(x, y, startOffset[1], startOffset[2]);
                break;
            case "down-right-bottom":
                resizeRight(x, y, startOffset[0]);
                resizeBottom(x, y, startOffset[1]);
                break;
        }
        timer.reset();
        if (callbackInstantaneous)
        {
            try
            {
                callbackInstantaneous();
            }
            catch(ex)
            {
                console.error(ex);
            }
        }
    }
    this.setBounds = function (boundsIn)//work in progress.
    {
        var previousBounds = bounds;
        bounds = boundsIn;
        if (previousBounds.minYpx < bounds.minYPx)
        {

        }
        if (previousBounds.maxYPx > bounds.maxYPx)
        {

        }
        if (previousBounds.minXPercent < bounds.minXPercentage)
        {

        }
        if (previousBounds.maxXPercent > bounds.maxXPercent)
        {

        }
    };
}
Resizer.padding = 3;
Resizer.WIDTH=6;
export default Resizer;

