import isNullOrUndefined from './isNullOrUndefined';
﻿export default class StringHelper {
	static addZerosEveryThreeDigitsOfNumber(number){
		let str = String(number);
		var newStr ='';
		var j=0;
		for(var i=str.length-1; i>=0; i--){
			if(j>2){
				j=0;
				newStr=','+newStr;
			}
			else j++;
			newStr=str[i]+newStr;
		}
		return newStr;
	}
	static multiSplit(str, toSplitOns){
		if(isNullOrUndefined(str))return str;
		let currentSplits = [str];
		for(var i=0; i<toSplitOns.length; i++){
			let newSplits = [];
			const toSplitOn = toSplitOns[i];
			for(var j=0; j<currentSplits.length; j++){
				const splits =  currentSplits[j].split(toSplitOn);
				newSplits = newSplits.concat(splits);
			}
			currentSplits = newSplits;
		}
		return currentSplits;
	}
}