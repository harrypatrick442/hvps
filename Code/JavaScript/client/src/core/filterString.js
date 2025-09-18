import isNullOrUndefined from './isNullOrUndefined';
export default function filterString(str, callback){
	if(isNullOrUndefined(str)){
		return str;
	}
	let newStr ='';
	for(let i=0; i<str.length; i++){
		const c = str[i];
		if(callback(c))
			newStr+=c;
	}
	return newStr;
}