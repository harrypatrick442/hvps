import E from './E';
export default function _createTextButton({className, callback, disposes, useMouseDown, text}){
	const buttonElement = document.createElement('button');
	if(className)
		buttonElement.classList.add(className);
	buttonElement.textContent = text;
	if(callback){
		buttonElement.addEventListener('click', callback);
		disposes.push(()=>buttonElement.removeEventListener('click', callback));
	}
	return buttonElement;
}