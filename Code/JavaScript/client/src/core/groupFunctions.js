export default function groupFunctions (){
	var toCalls=[];
	Array.from(arguments).forEach((argument)=>{
		if(argument)toCalls.push(argument);
	});
	if(toCalls.length==1)return toCalls[0];
	return function(){ 
		var args = arguments;
		toCalls.forEach((toCall)=>{
			toCall.apply(null, args);
		});
	};
};