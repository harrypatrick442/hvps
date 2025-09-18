import DynamicIcon from '../ui_core/DynamicIcon';
import isNullOrUndefined from '../core/isNullOrUndefined';
const mapNameToDynamicIcon = new Map();
const missings = new Set();
export default class Icons{
	static update(mappings){
		mappings.forEach(mapping=>{
			const [name, src]=mapping;
			let dynamicIcon = mapNameToDynamicIcon.get(name);
			if(isNullOrUndefined(dynamicIcon)){
				dynamicIcon = new DynamicIcon(src, name, Icons._handleMissing);
				mapNameToDynamicIcon.set(name, dynamicIcon);
			}
			else{
				dynamicIcon.src = src;
			}
		});
	}
	static get(name){
		let dynamicIcon = mapNameToDynamicIcon.get(name);
		if(isNullOrUndefined(dynamicIcon)){
			dynamicIcon =  new DynamicIcon(null, name, Icons._handleMissing);
			mapNameToDynamicIcon.set(name, dynamicIcon);
			return dynamicIcon;
		}
		return dynamicIcon;
	}
	static _handleMissing(dynamicIcon){
		if(missings.has(dynamicIcon))
			return;
		missings.add(dynamicIcon);
		if(Icons._setTimeout){
			return;
		}
		Icons._setTimeout = true;
		setTimeout(()=>{
			Icons._setTimeout = false;
			let str = 'MISSING ICONS [';
			let first = true;
			missings.forEach(dynamicIcon=>{
				if(!isNullOrUndefined(dynamicIcon.src))
					return;
				if(first)
					first = false;
				else
					str+=',';
				str+='[\'';
				str+=dynamicIcon.name;
				str+='\',\'';
				str+=dynamicIcon.name;
				str+='.png\']\n';
			});
			str+=']';
			if(first)return;
			console.log(str);
		}, 0);
	}
}