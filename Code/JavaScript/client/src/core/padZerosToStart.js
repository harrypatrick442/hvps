export default function padZerosToStart(str, length){
	if(!str)return str;
	while(str.length<length)
		str='0'+str;
	return str;
}