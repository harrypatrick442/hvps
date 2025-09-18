import E from './E';
import DynamicIcon from './DynamicIcon';
import _createMultipleStateImg from './_createMultipleStateImg';
import PageKeys from '../core/PageKeys';
export default function _createImageHoverTextButton(img, imgHover, className, callback, disposes, text, useMouseDown){
	const buttonElement = document.createElement('button');
	buttonElement.classList.add('image-hover-text-button');
	let hovering = false;
	let disposeEnterKeyRegistration;
	const [imgElement , setImg]= _createMultipleStateImg([img, imgHover], disposes);
	if(Array.isArray(className)){
		className.forEach(c=>buttonElement.classList.add(c));
	}
	else
		buttonElement.classList.add(className);
	if(callback){
		const eventName = useMouseDown?'mousedown':'click';
		buttonElement.addEventListener(eventName, callback);
		disposes.push(()=>{
			buttonElement.removeEventListener(eventName, callback);
		});
	}
	const enter = ()=>{
		setImg(1);
		buttonElement.classList.add('hovering');
		if(useMouseDown&&callback){
			if(disposeEnterKeyRegistration){
				disposeEnterKeyRegistration();
			}
			disposeEnterKeyRegistration = PageKeys.instance.registerSpecificKeyDown(13, callback);
		}
	};
	const leave = ()=>{
		setImg(0);
		if(disposeEnterKeyRegistration){
			disposeEnterKeyRegistration();
			console.log('deregistered');
			disposeEnterKeyRegistration = null;
		}
		buttonElement.classList.remove('hovering');
	};
	buttonElement.addEventListener('mouseenter', enter);
	buttonElement.addEventListener('mouseleave', leave);
	buttonElement.addEventListener('focus', enter);
	buttonElement.addEventListener('blur', leave);
	const textElement = E.div('text');
	textElement.textContent = text;
	const imgWrapperElement = E.div('img-wrapper');
	buttonElement.appendChild(imgWrapperElement);
	imgWrapperElement.appendChild(imgElement);
	buttonElement.appendChild(textElement);
	return buttonElement;
}