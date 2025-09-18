export default function capitalizeFirstLetter(str){
	return str.substr(0, 1).toUpperCase()+str.substr(1, str.length-1);
}