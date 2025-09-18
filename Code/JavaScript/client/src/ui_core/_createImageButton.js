import DynamicIcon from './DynamicIcon';
export default function _createImageButton(imgSrc, className, callback, disposes, useMouseDown){
	const button = document.createElement('button');
	const img = document.createElement('img');
	img.fill="currentColor";
	if(imgSrc instanceof(DynamicIcon)){
		const handleChanged = ({src})=>img.src = src;
		img.src = imgSrc.src;
		imgSrc.addEventListener('changed', handleChanged);
		disposes.push(()=>imgSrc.removeEventListener('changed', handleChanged));
	}
	else{
		img.src = imgSrc;
	}
	button.appendChild(img);
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
	return button;
}