import _createImagesHoverButton from '../ui_core/_createImagesHoverButton';
export default function _createImageHoverButton(img, imgHover, className, callback, disposes, useMouseDown, resets){
	const [button, setImg] = _createImagesHoverButton([img, imgHover], className, callback, disposes, useMouseDown, resets);
	const enter = ()=>setImg(1);
	const leave = ()=>setImg(0);
	button.addEventListener('mouseenter', enter);
	button.addEventListener('mouseleave', leave);
	button.addEventListener('focus', enter);
	button.addEventListener('blur', leave);
	resets?.push(leave);
	return button;
}