import isNullOrUndefined from '../core/isNullOrUndefined';
import UrlParameters  from '../urls/UrlParameters';
export default function debug(str){
	if(UrlParameters.debug
	//||window.location.port==='3000'
	)
	console.log(str);
}