import _saveFile from './_saveFile';
export default function _saveJSONFile(jObject, name){
	const jsonString = JSON.stringify(jObject);
	const blob = new Blob([jsonString], {type: 'text/json'});
	_saveFile(blob, name);
}