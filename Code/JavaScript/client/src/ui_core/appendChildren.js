export default function appendChildren(element, childElements){
	childElements.forEach(childElement=>element.appendChild(childElement));
}