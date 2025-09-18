import isNullOrUndefined from '../core/isNullOrUndefined';
export default function _parseErrorToString(e){
	if(isNullOrUndefined(e))return '';
	if(typeof(e)==='string')
		return e;
	if(!isNullOrUndefined(e.stack))
		return e.stack;
	if(!isNullOrUndefined(e.message))
		return e.message;
	if(!isNullOrUndefined(e.toString))
		return e.toString();
	return String(e);
}