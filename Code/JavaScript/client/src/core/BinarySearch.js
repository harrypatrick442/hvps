export default class BinarySearch{
	//preidcate should return <0 is lower, 0 is match, >0 is higher
	static binarySearch({items, compare, roundUpOnEquals, exactMatch}){
		const length = items.length;
		if(length<1)
			return null;
		let start = 0, end = length - 1;
		let mid;
		let greaterThanZero = false;
		while (start <= end) {
			mid = Math.floor((start + end) / 2);
			const middleItem = items[mid];
			const compareReturn = compare(middleItem);
			if (compareReturn===0) 
				return roundUpOnEquals?mid+1:mid; 
			greaterThanZero = compareReturn>0;
			if (greaterThanZero)
			{
				start = mid + 1;
				continue;
			}
			end = mid - 1;
		}
		if(exactMatch)return null;
		return greaterThanZero?mid+1:mid;
	}

}