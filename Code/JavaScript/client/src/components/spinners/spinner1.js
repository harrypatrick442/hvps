import E from '../../ui_core/E';
import spinner1Png from './spinner1.gif';
import './spinner1.css';
export default function spinner1(){
	//https://loading.io/asset/728992
	//loadingio-spinner-pulse-jq6xz2vkavb
	const element = E.div('spinner');
	element.classList.add('spinner-1');
	const img = E.img();
	element.appendChild(img);
	img.src = spinner1Png;
	return element;
}