import E from '../ui_core/E';
export default function _saveFile(blob, name){
	const a = E.a('temporary-for-save-file');
	a.href=window.URL.createObjectURL(blob);
	a.style.display = 'none';
	if(name)
		a.download=name;
	document.body.appendChild(a);
	a.click();
	document.body.removeChild(a);
}