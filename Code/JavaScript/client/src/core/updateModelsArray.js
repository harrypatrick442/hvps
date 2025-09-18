export default function updateModelsArray(existings, news, getId, createNew, updateExisting){
	if(!news||news.length<1){
		if(existings){
			existings.forEach(e=>e.dispose());
			return [];
		}
		return existings;
	}
	if(!existings||existings.length<1){
		existings = news.map(createNew);
		return existings;
	}
	const mapIdToNew = new Map();
	news.forEach(n=>mapIdToNew.set(getId(n), n));
	const mapIdToExisting = new Map();
	existings.forEach(e=>mapIdToExisting.set(getId(e), e));
	existings.forEach(e=>{
		const id = getId(e);
		const n = mapIdToNew.get(id);
		if(n){
			mapIdToNew.delete(id);
			updateExisting(e, n);
			return;
		}
		e.dispose();
		mapIdToExisting.delete(id);
	});
	mapIdToNew.values().forEach(n=>mapIdToExisting.set(getId(n), createNew(n)));
	return Array.from(mapIdToExisting.values());
}
		