import isNullOrUndefined from './isNullOrUndefined';
export default function escapeTags(text){
	if(isNullOrUndefined(text))return text;
	return text.replace(/<|>/g, '') 
}