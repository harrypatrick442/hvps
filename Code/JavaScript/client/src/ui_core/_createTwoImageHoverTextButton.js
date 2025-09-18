import E from './E';
import DynamicIcon from './DynamicIcon';
import _createMultipleStateImg from './_createMultipleStateImg';
export default function _createTwoImageHoverTextButton(img, imgHover, className, callback, disposes, text, useMouseDown){
	const buttonElement = document.createElement('button');
	buttonElement.classList.add('image-hover-text-button');
	let hovering = false;
	const [imgElement , setImg]= _createMultipleStateImg([img, imgHover], disposes);
	const [imgElement2 , setImg2]= _createMultipleStateImg([img, imgHover], disposes);
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
	buttonElement.addEventListener('mouseenter', ()=>{
		setImg(1);
		setImg2(1);
		buttonElement.classList.add('hovering');
	});
	buttonElement.addEventListener('mouseleave', ()=>{
		setImg(0);
		setImg2(0);
		buttonElement.classList.remove('hovering');
	});
	const textElement = E.div('text');
	textElement.textContent = text;
	const imgWrapperElement = E.div('img-wrapper');
	imgWrapperElement.classList.add('left');
	const imgWrapperElement2 = E.div('img-wrapper');
	imgWrapperElement2.classList.add('right');
	buttonElement.appendChild(imgWrapperElement);
	imgWrapperElement.appendChild(imgElement);
	buttonElement.appendChild(textElement);
	buttonElement.appendChild(imgWrapperElement2);
	imgWrapperElement2.appendChild(imgElement2);
	return buttonElement;
}