export default function a(){
	const parent = arguments[0];
	for(var i = 1; i < arguments.length; i++)
	{
		parent.appendChild(arguments[i]);
	}
	return parent;
}