﻿
export default class HashSource {
	static next(){
		const next =String(HashSource._count++);
		return next;
	}
}
HashSource._count=0;