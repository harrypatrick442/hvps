import i from './i';
import E from './E';
export default function _dynamicIcon(className, iconName, disposes){
	const d = i(iconName);
	const imgElement = E.img(className);
	imgElement.src = d.src;
	const handleChanged = ({src})=>{
		imgElement.src = src;
	};
	d.addEventListener('changed', handleChanged);
	disposes.push(()=>d.removeEventListener('changed', handleChanged));
	return imgElement;
}