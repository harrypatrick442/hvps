export default function _createButton(text, className, callback, disposes){
	const button = document.createElement('button');
	button.textContent = text;
	button.classList.add(className);
	if(callback){
		button.addEventListener('click', callback);
		disposes.push(()=>{
			button.removeEventListener('click', callback);
		});
	}
	return button;
}