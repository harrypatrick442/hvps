import E from './E';
import DynamicIcon from './DynamicIcon';
export default function _createMultipleStateImg(imgs, disposes, className){
	const imgElement = document.createElement('img');
	if(className){
		if(Array.isArray(className))
			className.forEach(e=>imgElement.classList.add(e));
		else
			imgElement.classList.add(className);
	}
	imgElement.fill="currentColor";
	let i = 0;
	let activeIndex;
	const mappings = imgs.map(img=>{
		const index = i++;
		if(img instanceof(DynamicIcon)){
			const load = ()=>{
				activeIndex = index;
				imgElement.src = img.src;
			};
			const handleChanged = ({src})=>{
				if(activeIndex === index){
					load();
				}
			};
			const removeChanged = img.addEventListener('changed', handleChanged);
			disposes.push(removeChanged);
			return [img.src, load];
		}
		else{
			const load = ()=>{
				activeIndex = index;
				imgElement.src = img;
			};
			return [img, load];
		}
	});
	const setImg = (index)=>{
		mappings[index][1]();
	};
	setImg(0);
	return [imgElement, setImg];
}