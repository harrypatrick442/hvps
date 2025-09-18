import HashSource from './HashSource';
var count=1;
const HashBuilder = function(obj){
	if(obj.hash)return obj.hash;
	obj.hash = HashSource.next();
	return obj.hash;
};
export default HashBuilder;
	