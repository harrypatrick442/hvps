export default function getSetCssClassFromValue(getCssClassFromValue, element){
	let currentClass;
	return (value)=>{
		if(currentClass)
			element.classList.remove(currentClass);
		currentClass = getCssClassFromValue(value);
		if(currentClass)
			element.classList.add(currentClass);
	};
}