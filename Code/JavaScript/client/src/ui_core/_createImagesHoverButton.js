import DynamicIcon from './DynamicIcon';
import _createMultipleStateImg from './_createMultipleStateImg';
export default function _createImagesHoverButton(imgs, className, callback, disposes, useMouseDown){
	const button = document.createElement('button');
	button.classList.add('image-hover-button');
	const [imgElement, setImg] = _createMultipleStateImg(
	imgs, disposes, 'img');
	imgElement.fill="currentColor";
	button.appendChild(imgElement);
	if(Array.isArray(className)){
		className.forEach(c=>button.classList.add(c));
	}
	else
		button.classList.add(className);
	if(callback){
		const eventName = useMouseDown?'mousedown':'click';
		button.addEventListener(eventName, callback);
		disposes.push(()=>{
			button.removeEventListener(eventName, callback);
		});
	}
	return [button, setImg];
}