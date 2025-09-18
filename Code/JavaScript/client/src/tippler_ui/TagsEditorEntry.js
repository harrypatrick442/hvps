import E from '../ui_core/E';
import i from '../ui_core/i';
import _createImageHoverButton from '../ui_core/_createImageHoverButton';
export default function TagsEditorEntry({str, disposed, remove}){
	const self = this;
	const disposes =[];
	self.str = str;
	self.hash = str;
	self.disposed = disposed;
	const element = (self.element = E.div('entry'));
	const strElement = E.div('str');
	strElement.textContent = str;
	const removeButton = _createImageHoverButton(
		i('Close2'), i('CloseHover'), 'remove-button', ()=>remove(str), disposes, true
	);
	element.appendChild(strElement);
	element.appendChild(removeButton);
	self.dispose = ()=>{
		const parent = element.parentNode;
		if(parent)parent.removeChild(element);
		disposes.forEach(d=>d());
		disposed(self);
	};
	self.highlightDuplicate = ()=>{
		let interval;
		let flashCount = 0;
		let flashing = false;
		const setNotFlashing = ()=>
				element.classList.remove('duplicate-flashing');
		const clearTheInterval = ()=>{
			if(interval){
				clearInterval(interval);
				interval = null;
			}
		};
		interval = setInterval(()=>{
			if(flashing){
				setNotFlashing();
				flashCount++;
			}
			else
				element.classList.add('duplicate-flashing');
			flashing=!flashing;
			if(flashCount>=2)
			{
				clearTheInterval();
				setNotFlashing();
			}
		}, 60);
		element.classList.add('duplicate');
		return ()=>{
			console.error(new Error().stack);
			element.classList.remove('duplicate');
			clearTheInterval();
			setNotFlashing();
		};
	};
}