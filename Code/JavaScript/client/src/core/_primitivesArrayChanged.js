import isNullOrUndefined from './isNullOrUndefined';
export default function _primitivesArrayChanged(a, b){
	if(isNullOrUndefined(a)){
		return !isNullOrUndefined(b);
	}
	if(isNullOrUndefined(b))return true;
	if(a.length!==b.length)return true;
	for(var i=0; i<a.length; i++){
		if(a[i]!==b[i])return true;
	}
	return false;
}