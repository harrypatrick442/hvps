import E from './E';
import DynamicIcon from './DynamicIcon';
export default function _createImageTextButton({imgSrc, className, callback, disposes, useMouseDown, text}){
	const buttonElement = document.createElement('button');
	if(className)
		buttonElement.classList.add(className);
	const imgElement = document.createElement('img');
	if(imgSrc instanceof(DynamicIcon)){
		const handleChanged = ({src})=>imgElement.src = src;
		imgElement.src = imgSrc.src;
		imgSrc.addEventListener('changed', handleChanged);
		disposes.push(()=>imgSrc.removeEventListener('changed', handleChanged));
	}
	else{
		imgElement.src = imgSrc;
	}
	const textElement = E.div('text');
	textElement.textContent = text;
	const imgWrapperElement = E.div('img-wrapper');
	buttonElement.appendChild(imgWrapperElement);
	imgWrapperElement.appendChild(imgElement);
	buttonElement.appendChild(textElement);
	if(callback){
		buttonElement.addEventListener('click', callback);
		disposes.push(()=>buttonElement.removeEventListener('click', callback));
	}
	return buttonElement;
}