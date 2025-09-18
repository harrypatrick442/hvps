export default function _addElseRemoveClass(addElseRemove, className, element){
		if(addElseRemove)
			element.classList.add(className);
		else
			element.classList.remove(className);
}