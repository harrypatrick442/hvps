import DynamicIcon from './DynamicIcon';
import _addElseRemoveClass from './_addElseRemoveClass';
export default function _createImageHoverButton({
		img, imgHover, imgActiveHover, imgActive, className, callback, disposes, useMouseDown, resets
	}){
	const button = document.createElement('button');
	button.classList.add('image-hover-button');
	const imgElement = document.createElement('img');
	imgElement.fill="currentColor";
	let hovering = false; let active = false;
	let imgSrc, imgHoverSrc, imgActiveSrc, imgActiveHoverSrc;
	const getSrcForState = ()=>{
		if(hovering){
			if(active){
				if(imgActiveHoverSrc)
					return imgActiveHoverSrc;
			}
			if(imgHoverSrc)
				return imgHoverSrc;
		}
		if(active){
			if(imgActiveSrc)
				return imgActiveSrc;
		}
		return imgSrc;
	};
	const updateSrc = ()=>{
		imgElement.src = getSrcForState();
	};
	const setupSrc = (img, setSrc)=>{
		if(img instanceof(DynamicIcon)){
			const handleChanged = ({src})=>{
				setSrc(src);
				updateSrc();
			};
			disposes.push(img.addEventListener('changed', handleChanged));
			setSrc(img.src);
		}
		else{
			setSrc(img);
		}
	};
	setupSrc(img, (src)=>imgSrc = src);
	setupSrc(imgHover, (src)=>imgHoverSrc = src);
	setupSrc(imgActiveHover, (src)=>imgActiveHoverSrc = src);
	setupSrc(imgActive, (src)=>imgActiveSrc = src);
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
	button.addEventListener('mouseenter', ()=>{
		hovering = true;
		updateSrc();
	});
	const handleMouseLeave = ()=>{
		hovering = false;
		updateSrc();
	};
	button.addEventListener('mouseleave', handleMouseLeave);
	resets?.push(handleMouseLeave);
	const setActive = (value)=>{
		active = value;
		_addElseRemoveClass(active, 'active', button);
		_addElseRemoveClass(hovering, 'hovering', button);
		updateSrc();
	};
	updateSrc();
	return [button, setActive];
}