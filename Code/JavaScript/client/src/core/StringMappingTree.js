import isNullOrUndefined from './isNullOrUndefined';
export default class StringMappingTree{
	constructor(){
		this.add = this.add.bind(this);
		this.get = this.get.bind(this);
		this._map = new Map();
	}
	
	add(str, obj){
		let map = this._map;
		let child;
		for(var c of str.substr(0, str.length-1)){
			let child = map.get(c);
			if(!child){
				child = {
					map:new Map()
				};
				map.set(c, child);
				map = child.map;
				continue;
			}
			map = child.map;
			if(!map){
				map = (child.map= new Map());
			}
		}
		c = str[str.length-1];
		child = map.get(c);
		if(child){
			if(child.obj){
				throw new Error(`duplicated object on path ${str}`);
			}
			child.obj = obj;
			return;
		}
		child = {
			obj
		};
		map.set(c,child);
	}
	get(str, i){
		let map = this._map;
		let nCharacters=1;
		let finds=null;
		if(isNullOrUndefined(i)){
			i=0;
		}
		while(i< str.length){
			const c = str[i++];
			const child = map.get(c);
			if(!child) {
				return finds;
			}
			const obj = child.obj;
			if(obj){
				if(finds===null)
					finds = [];
				finds.push([obj, nCharacters]);
			}
			map = child.map;
			if(!map){
				return finds;
			}
			nCharacters++;
		}
		return finds;
	}
}