import _saveFile from './_saveFile';
import _pickFile from './_pickFile';
import isNullOrUndefined from './isNullOrUndefined';
export default function _loadJSONFile(jObject, name){
	return new Promise((resolve, reject)=>{
		_pickFile().then((file)=>{
			if (isNullOrUndefined(file)){
				resolve(null);
				return;
			} 
			const reader = new FileReader();
			reader.addEventListener(
				"load",
				() => {
					resolve(JSON.parse(reader.result));
				},
				false,
			);
			reader.addEventListener(
				"error",
				reject,
				false,
			);
			reader.readAsText(file);
		}).catch(reject);
	});
}