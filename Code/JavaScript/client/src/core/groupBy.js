export default function groupBy(entries, callback){
	const map = new Map();
	entries.forEach(e=>{
		const key = callback(e);
		let group = map.get(key);
		if(group)
		{
			group.push(e);
		}
		else
		{
			group = [e];
			map.set(key, group);
		}
	});
	return Array.from(map.values());
}