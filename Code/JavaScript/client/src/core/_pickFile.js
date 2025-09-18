import E from '../ui_core/E';
import isNullOrUndefined from '../core/isNullOrUndefined';
export default function _pickFile(e){
	const {accept}=e||{};
	return new Promise((resolve, reject)=>{
		const inputElement = E.file();
		if(accept)
			inputElement.accept = accept;
		const cleanup = ()=>{
			inputElement.removeEventListener('cancel', cancelled);
			inputElement.removeEventListener('change', onFileChanged);
			document.body.removeChild(inputElement);
		};
		let onFileChanged, cancelled;
		onFileChanged = (e)=>{
			const file = inputElement.files[0];
			resolve(file);
			cleanup();
		};
		cancelled = (e)=>{
			cleanup();
			resolve(null);
		};
		inputElement.style.display='none';
		document.body.appendChild(inputElement);
		inputElement.addEventListener('cancel', cancelled);
		inputElement.addEventListener('change', onFileChanged);
		inputElement.click();
	});
}