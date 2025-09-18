import HashSource from './HashSource';
export default function enableHash(obj){
	if(obj.hash!==undefined&&obj.hash!==null)return obj.hash;
	obj.hash = HashSource.next();
	return obj.hash;
}