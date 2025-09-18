import getStyle from './getStyle';
export default function isDisplaying(element){
	let currentElement = element;
	while(currentElement){
		if(getStyle(currentElement, 'display')==='none')return false;
		currentElement = currentElement.parentElement;
	}
	return true;
}