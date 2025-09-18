export default function joinStringsWithCommasAnd(strings){
	if(strings.length<1)return '';
	const str =strings[0];
	if(strings.length<2)
		return str;
	var i=1;
	while(i<strings.length-1){
		str+=`, ${strings[i++]}`;
	}
	str+=' and '+strings[i];
}