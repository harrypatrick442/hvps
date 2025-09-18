import isNullOrUndefined from './isNullOrUndefined';
export default function arrayCompare(a, b){
	if(isNullOrUndefined(a))
	{
		if(!isNullOrUndefined(b))return false;
		return a===b;
	}
	if(isNullOrUndefined(b))
		return false;
	if(a.length!=b.length)return false;
	for(var i=0; i<a.length; i++){
		if(a[i]!==b[i])return false;
	}
	return true;
}