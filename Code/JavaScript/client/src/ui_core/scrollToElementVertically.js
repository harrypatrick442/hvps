import Alignment from '../enums/Alignment';
import isNullOrUndefined from '../core/isNullOrUndefined';
import getAbsoluteRectangle from './getAbsoluteRectangle';
export default function scrollToElementVertically({scrollingElement, element, alignment}){
	const sA = getAbsoluteRectangle(scrollingElement);
	const eA = getAbsoluteRectangle(element);
	let eAT;
	let desiredYAbsolute;
	if(isNullOrUndefined(alignment)){
		if(eA.t + eA.h > sA.t + sA.h){
			alignment = Alignment.Bottom;
		}
		else{
			if(eA.t >= sA.t){
				return;
			}
			alignment = Alignment.Top;
		}
	}
	switch(alignment){
		case Alignment.Top:
			desiredYAbsolute = sA.t;
			eAT = eA.t;
			break;
		case Alignment.Bottom:
			desiredYAbsolute = sA.t+sA.h;
			eAT = eA.t + eA.h;
			break;
		case Alignment.Middle:
		default:
			desiredYAbsolute = sA.t+(sA.h/2);
			eAT = eA.t + (eA.h/2);
			break;
	}
	const scrollTop = scrollingElement.scrollTop;
	const dY = desiredYAbsolute - eAT;
	let newScrollTop = scrollTop - dY;
	let b;
	
	if(newScrollTop <0)
	{
		newScrollTop=0;
	}
	else if(newScrollTop > (b = scrollingElement.scrollHeight -  scrollingElement.offsetHeight)){
		newScrollTop = b;
	}
	scrollingElement.scrollTop = newScrollTop;
}