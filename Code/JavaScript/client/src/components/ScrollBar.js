import ParameterErrors from '../errors/ParameterErrors';
import eventEnable from '../core/eventEnable';
import i from '../ui_core/i';
import isNullOrUndefined from '../core/isNullOrUndefined';
import Handles from '../core/Handles';
import E from '../ui_core/E';
import ResizeWatchers from '../ui_core/ResizeWatchers';
import EfficientMoveCycle from '../ui_core/EfficientMoveCycle';
import LoopBreak from '../core/LoopBreak';
import ClickedOff from '../ui_core/ClickedOff';
import KeyCode from '../enums/KeyCode';
import NativeAPI from '../api/NativeAPI';
import Platform from '../enums/Platform';
import HandledManager from '../core/HandledManager';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
import './ScrollBar.scss';
let scrollByKeysHandled = false;
const { throwNotProvided } = ParameterErrors;
const PROPORTION_VISIBLE_PER_ARROW_KEY_PRESS = 0.08, PROPORTION_VISIBLE_PER_MOUSE_SHAFT = 0.3;
export default function Scrollbar({horizontalElseVertical, 
	elementBeingScrolled, positionedRelative}) {
	const self = this;
	const disposes = [];
	/*if(isNullOrUndefined(scrollOnClickShaft))
		scrollOnClickShaft = true;*/
	const element = (self.element = E.div('scroll-bar'));
	element.classList.add(horizontalElseVertical?'horizontal':'vertical');
	const innerElement = E.div('inner');
	const barElement = E.div('bar');
	const barInnerElement = E.div('inner');
	let incrementalScroll;
	const buttonTowards = _createImageHoverButton( 
		horizontalElseVertical?i('ScrollBarArrowHeadLeft'):i('ScrollBarArrowHeadUp'),
		horizontalElseVertical?i('ScrollBarArrowHeadLeftHover'):i('ScrollBarArrowHeadUpHover'),
		'icon-towards',
		()=>incrementalScroll(false, PROPORTION_VISIBLE_PER_MOUSE_SHAFT), 
		disposes, 
		true
	);
	const buttonAway = _createImageHoverButton( 
		horizontalElseVertical?i('ScrollBarArrowHeadRight'):i('ScrollBarArrowHeadDown'),
		horizontalElseVertical?i('ScrollBarArrowHeadRightHover'):i('ScrollBarArrowHeadDownHover'), 
		'icon-away',
		()=>incrementalScroll(true, PROPORTION_VISIBLE_PER_MOUSE_SHAFT), 
		disposes,
		true
	);
	element.appendChild(innerElement);
	element.appendChild(barElement);
	barElement.appendChild(barInnerElement);
	element.appendChild(buttonTowards);
	element.appendChild(buttonAway);
	const loopBreak = new LoopBreak();
	const wFromE = horizontalElseVertical?(e)=>e.x:(e)=>e.y;
	const getElementSize = horizontalElseVertical?()=>element.offsetWidth:()=>element.offsetHeight;
	const getBarSize = horizontalElseVertical?()=>barElement.offsetWidth:()=>barElement.offsetHeight;
	const getBarW = horizontalElseVertical?()=>barElement.offsetLeft:()=>barElement.offsetTop;
	const setBarW = horizontalElseVertical?(w)=>barElement.style.left=`${w}px`:(w)=>{
		barElement.style.top=`${w}px`;
	};
	const getElementBeingScrolledFullSize = horizontalElseVertical?()=>elementBeingScrolled.scrollWidth:()=>elementBeingScrolled.scrollHeight;
	const getElementBeingScrolledVisibleSize = horizontalElseVertical?()=>elementBeingScrolled.clientWidth:()=>elementBeingScrolled.clientHeight;
	const getScrollW = horizontalElseVertical?()=>elementBeingScrolled.scrollLeft:()=>elementBeingScrolled.scrollTop;
	const setScrollW = horizontalElseVertical?(s)=>elementBeingScrolled.scrollLeft = s:(s)=>elementBeingScrolled.scrollTop = s;
	const getOffsetOnElement = horizontalElseVertical?(e)=>{
		const rect = element.getBoundingClientRect();
		const elementTop = rect.left + window.pageXOffset - document.documentElement.clientLeft;
		return e.x - elementTop;
	}:(e)=>{
		const rect = element.getBoundingClientRect();
		const elementTop = rect.top + window.pageYOffset - document.documentElement.clientTop;
		return e.y - elementTop;
	};
	let startMouseW, startBarW, latestMouseW, distanceBarCanMove, proportionScrolled=0,
		startProportionScrolled, active, hidden,
		draggingSuspendHandle, shaftSuspendHandle, handleExternalScroll;
	
	const registerScrollEvent = ()=>{
		if(self._registeredScrollEvent)return;
		elementBeingScrolled.addEventListener('scroll', handleExternalScroll);
		self._registeredScrollEvent = true;
	};
	const unregisterScrollEvent = ()=>{
		if(!self._registeredScrollEvent)return;
		elementBeingScrolled.removeEventListener('scroll', handleExternalScroll);
		self._registeredScrollEvent = false;
	};
	const suspendScrollEventHandles = new Handles({onChange:(suspended)=>{
		if(suspended)
			unregisterScrollEvent();
		else
			registerScrollEvent();
	}});
	const updateHidden = ()=>{
		if((hidden=getElementBeingScrolledFullSize()<=getElementBeingScrolledVisibleSize())){
			element.classList.add('hidden');
			return true;
		}
		element.classList.remove('hidden');
		return false;
	};
	const updateDistanceBarCanMove = ()=>{
		distanceBarCanMove = getElementSize() - getBarSize();
	};
	const scrollElementToProportion = (p)=>{
		const sizeToScroll/*scrollTop or scrollLeft*/ 
			= getElementBeingScrolledFullSize() - getElementBeingScrolledVisibleSize();
		const scrollW = sizeToScroll *p;
		setScrollW(scrollW);
	};
	const onStartDragBar = (e)=>{
		draggingSuspendHandle?.dispose();
		draggingSuspendHandle = suspendScrollEventHandles.take();
		updateHidden();
		updateDistanceBarCanMove();
		loopBreak.trigger();
		startMouseW = wFromE(e);
		startBarW = getBarW();
	}
	const onMoveDragBar = (e)=>{
		if(hidden)return;
		latestMouseW = wFromE(e);
		const dW = latestMouseW - startMouseW;
		let newBarW = dW+startBarW;
		if(newBarW<0)newBarW = 0;
		else
		if(newBarW>distanceBarCanMove)
			newBarW = distanceBarCanMove;
		proportionScrolled = distanceBarCanMove<=0?0:newBarW / distanceBarCanMove;
		setBarW(newBarW);
		scrollElementToProportion(proportionScrolled);
	};
	const onEndDragBar = (e)=>{
		draggingSuspendHandle?.dispose();
		draggingSuspendHandle = null;
	};
	const getProportionScrolledFromElementBeingScrolled = ()=>{
		const denominator = getElementBeingScrolledFullSize() - getElementBeingScrolledVisibleSize();
		if(denominator<=0)
			return 0;
		return getScrollW()/denominator;
	};
	const scrolledBySomethingOtherThanUser = ()=>{
		updateDistanceBarCanMove();
		updateHidden();
		proportionScrolled = getProportionScrolledFromElementBeingScrolled();
		startProportionScrolled = proportionScrolled;
		if(proportionScrolled<0)proportionScrolled = 0;
		else if(proportionScrolled>1)proportionScrolled = 1;
		startBarW = distanceBarCanMove*proportionScrolled;
		setBarW(startBarW);
		startMouseW = latestMouseW;
	};
	const onResized = (fixProportion)=>{
		if(fixProportion){
			updateHidden();
			updateDistanceBarCanMove();
			startBarW = distanceBarCanMove*proportionScrolled;
			setBarW(startBarW);
			scrollElementToProportion(proportionScrolled);
			return;
		}
		updateDistanceBarCanMove();
		scrolledBySomethingOtherThanUser();
	};
	handleExternalScroll = ()=>{
		if(!self._registeredScrollEvent)return;
		scrolledBySomethingOtherThanUser();
	};
	/*
	const onResized = ()=>{
		updateHidden();
		updateDistanceBarCanMove();
		scrolledBySomethingOtherThanUser();
	};*/
	const efficientMoveCycle = new EfficientMoveCycle({
		element:barElement,
		preventDefault:true, onStart:onStartDragBar, onMove:onMoveDragBar, 
		onEnd:onEndDragBar, friendlyXY:true});
	disposes.push(efficientMoveCycle.dispose);
	self.scrolledBySomethingOtherThanUser = scrolledBySomethingOtherThanUser;
	self.resize = onResized;
	/*
	ResizeWatchers.add({element:elementBeingScrolled, onResized:()=>{
		onResized(true);
	}, staggered:false});
*/	ResizeWatchers.add({
		element, 
		onResized:
		//make sure no parameter causes fix
		()=>onResized(),
		staggered:false
	});
	
	
	incrementalScroll = (downElseUp, proportionVisible)=>{
		const dProportion = getElementBeingScrolledVisibleSize()*proportionVisible/getElementBeingScrolledFullSize();
		proportionScrolled = proportionScrolled+((downElseUp?1:-1)*dProportion);
		if(proportionScrolled<0)proportionScrolled = 0;
		else if(proportionScrolled>1)proportionScrolled = 1;
		setBarW(distanceBarCanMove*proportionScrolled);
		scrollElementToProportion(proportionScrolled);
	};
	let offsetOnElement;
	const getDownElseUp = ()=>{
		const barW = getBarW();
		const barSize = getBarSize();
		const middleBar = barW+(barSize/2);
		return offsetOnElement>middleBar;
	};
	let intervalHoldingMouseDownOnElement, cancelMouseDownInterval;
	const clearIntervalHoldingMouseDown = ()=>{
		shaftSuspendHandle?.dispose();
		shaftSuspendHandle = null;
		if(cancelMouseDownInterval){
			cancelMouseDownInterval();
			cancelMouseDownInterval = null;
		}
		if(!intervalHoldingMouseDownOnElement)return;
		clearInterval(intervalHoldingMouseDownOnElement);
		intervalHoldingMouseDownOnElement = null;
	};
	const mouseDownOnElement = (e)=>{
		if(hidden)return;
		if(loopBreak.triggered)return;
		shaftSuspendHandle?.dispose();
		shaftSuspendHandle = suspendScrollEventHandles.take();
		if(e.target!==element&&e.target!==innerElement)return;
		updateDistanceBarCanMove();
		if(distanceBarCanMove<=0)return;
		offsetOnElement = getOffsetOnElement(e);
		const downElseUp = getDownElseUp();
		incrementalScroll(downElseUp, PROPORTION_VISIBLE_PER_MOUSE_SHAFT);
		if(cancelMouseDownInterval){
			cancelMouseDownInterval();
			cancelMouseDownInterval = null;
		}
		let cancelled = false;
		cancelMouseDownInterval = ()=>cancelled = true;
		setTimeout(()=>{
			if(cancelled){
				shaftSuspendHandle?.dispose();
				shaftSuspendHandle = null;
				return;
			}
			intervalHoldingMouseDownOnElement = setInterval(()=>{
				if(downElseUp!==getDownElseUp()){
					clearIntervalHoldingMouseDown();
					return;
				}
				incrementalScroll(downElseUp, PROPORTION_VISIBLE_PER_MOUSE_SHAFT);
			}, 100);
		}, 500);
		
	};
	const efficientMoveCycleOnElement = new EfficientMoveCycle({
		element,
		preventDefault:true, onStart:mouseDownOnElement, onMove:(e)=>{
			if(hidden)return;
			offsetOnElement = getOffsetOnElement(e);
		}, 
		onEnd:clearIntervalHoldingMouseDown, friendlyXY:true});
	disposes.push(efficientMoveCycleOnElement.dispose);
	element.addEventListener('mouseleave', clearIntervalHoldingMouseDown);
	
		
		
	setTimeout(onResized, 0);
	const handleKeyPress =  ({keyCode})=>{
		if(HandledManager.handledKeyCode(keyCode))return;
		switch(keyCode){
			case KeyCode.ARROW_UP:
				incrementalScroll(false, PROPORTION_VISIBLE_PER_ARROW_KEY_PRESS);
				return;
			case KeyCode.ARROW_DOWN:
				incrementalScroll(true, PROPORTION_VISIBLE_PER_ARROW_KEY_PRESS);
				return;
			default:
				return;
		}
	};
	if(Platform.isMobile(NativeAPI.platform)||("ontouchstart" in document.documentElement)){
		const onStartDragElementBeingScrolled = (e)=>{
			draggingSuspendHandle?.dispose();
			draggingSuspendHandle = suspendScrollEventHandles.take();
			updateHidden();
			updateDistanceBarCanMove();
			loopBreak.trigger();
			startMouseW = wFromE(e);
			startBarW = getBarW();
			startProportionScrolled = proportionScrolled;
		}
		const onMoveDragElementBeingScrolled = (e)=>{
			if(hidden)return;
			latestMouseW = wFromE(e);
			const dW = latestMouseW - startMouseW;
			const denominator = elementBeingScrolled.scrollHeight - elementBeingScrolled.offsetHeight;
			proportionScrolled =  denominator <=0? startProportionScrolled:(startProportionScrolled - (dW/denominator));
			if(proportionScrolled<0)proportionScrolled= 0;
			else if(proportionScrolled>1)
				proportionScrolled = 1;
			setBarW(distanceBarCanMove*proportionScrolled);
			scrollElementToProportion(proportionScrolled);
		};
		const efficientMoveCycleDragElementBeingScrolled = new EfficientMoveCycle({
			element:elementBeingScrolled,
			preventDefault:false, onStart:onStartDragElementBeingScrolled, onMove:onMoveDragElementBeingScrolled, 
			onEnd:onEndDragBar, friendlyXY:true});
		disposes.push(efficientMoveCycleDragElementBeingScrolled.dispose);
	}
	else{
		const handleWheel = (e) => {
			if(hidden)return;
			e = e||window.event;
			const {deltaY} = e;
			const dProportion = deltaY/getElementBeingScrolledFullSize();
			proportionScrolled = proportionScrolled+dProportion;
			if(proportionScrolled<0)proportionScrolled = 0;
			else if(proportionScrolled>1)proportionScrolled = 1;
			setBarW(distanceBarCanMove*proportionScrolled);
			scrollElementToProportion(proportionScrolled);
		};
		elementBeingScrolled.addEventListener("wheel", handleWheel);
		element.addEventListener("wheel", handleWheel);
		disposes.push(()=>{
			elementBeingScrolled.removeEventListener("wheel", handleWheel);
			element.removeEventListener("wheel", handleWheel);
		});
	}
	self.setActive = (value)=>{
		active = value;
		if(value){
			window.addEventListener("keydown", handleKeyPress);
		}
		else{
			window.removeEventListener("keydown", handleKeyPress);
		}
	};
}