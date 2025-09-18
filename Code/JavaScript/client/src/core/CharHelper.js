import isNullOrUndefined from './isNullOrUndefined';
const NUMBER_CHARS = new Set();
for(let i=0; i<10; i++){
	NUMBER_CHARS.add(String(i));
}
﻿export default class CharHelper {
	static isNumber(c){
		c=c[0];
		return NUMBER_CHARS.has(c);
	}
}