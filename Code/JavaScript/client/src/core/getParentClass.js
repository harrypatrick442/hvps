export default function getParentClass(targetClass){
	if(targetClass instanceof Function){
		const parentClass = Object.getPrototypeOf(targetClass);
		if(parentClass && parentClass !== Object && parentClass.name){
			return parentClass;
		}
	}
	return null;
}