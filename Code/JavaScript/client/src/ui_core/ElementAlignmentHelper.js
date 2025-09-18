import getAbsolute from '../ui_core/getAbsolute';
import Alignment from '../ui_core/Alignment';
export default class ElementAlignmentHelper{
	static get_SetPositionRelativeToOtherElement(element, rightAlign){
		const setAlignment = ElementAlignmentHelper._getSetAlignment(element);
		return (elementShowingFor)=>{
			element.style.left='0px';
			element.style.top='0px';
			const width = element.offsetWidth;
			const absolutePositionElementShowingFor = getAbsolute(elementShowingFor);
			const leftWhichWouldBeRequiredForLeftAlign = absolutePositionElementShowingFor.left- element.offsetWidth;
			let left;
			let alignment = Alignment.left;
			if(leftWhichWouldBeRequiredForLeftAlign<0||rightAlign){
				alignment = Alignment.right;
				const leftWhichWouldBeRequiredForRightAlign = absolutePositionElementShowingFor.left+elementShowingFor.offsetWidth/*+ elementShowingFor.offsetWidth+*/;
				left = leftWhichWouldBeRequiredForRightAlign;
			}
			else{
				left = leftWhichWouldBeRequiredForLeftAlign;
			}
			setAlignment(alignment);
			element.style.left=String(left)+'px';
			element.style.top=String(absolutePositionElementShowingFor.top+ (elementShowingFor.offsetHeight/2) - (element.offsetHeight/2))+'px';
		};
	}
	static _getSetAlignment(element){
		let currentAlignmentClass = null;
		return (alignment)=>{
			const alignmentClass = alignment===Alignment.left?'align-left':'align-right';
			if(currentAlignmentClass===alignmentClass)return;
			element.classList.remove(currentAlignmentClass);
			element.classList.add(alignmentClass);
			currentAlignmentClass=alignmentClass;
		}
	}
}