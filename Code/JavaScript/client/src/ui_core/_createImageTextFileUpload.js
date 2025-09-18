import E from './E';
import _createImageTextButton from './_createImageTextButton';
import NativeAPI from '../api/NativeAPI';
//var _createNativeImageTextFileUpload, _createJavascriptImageTextFileUpload;
/*if(false&&NativeAPI.isNative){
	_createNativeImageTextFileUpload =function({imgSrc, className, callback, disposes, useMouseDown, text}){
		const pickFileNative=()=>{
			const identifier = pickFileResponse.fileIdentifier;
			NativeAPI.pickFile().then((pickFileResponse)=>{
				callback({file:{
					isNative:true, name:pickFileResponse.fileName, 
					size:pickFileResponse.fileSize, type:pickFileResponse.fileType, 
					identifier,
					writeData:NativeAPI.get_writeDataToFile(identifier),
					readData:NativeAPI.get_readDataToFile(identifier)
				}});
			}).catch(console.error);
		};
		return _createImageTextButton({imgSrc, className, callback:pickFileNative , disposes, useMouseDown, text});
	};
}*/
//_createJavascriptImageTextFileUpload =
export default function _createImageTextFileUpload({imgSrc, className, callback, disposes, useMouseDown, text}){
	const inputElement = E.file('file-input');
	const showFileOpenDialog = ()=> {
		inputElement.click();
	};
	const onFileChanged = (e)=>{
		const file = inputElement.files[0];
		inputElement.value = '';
		callback({file});
	};
	inputElement.addEventListener('change', onFileChanged);
	disposes.push(()=>inputElement.removeEventListener('change', onFileChanged));
	return _createImageTextButton({imgSrc, className, callback:showFileOpenDialog , disposes, useMouseDown, text});
};
/*
export default function _createImageTextFileUpload(params){
	if(params.canUseNative&&_createNativeImageTextFileUpload)
		return _createNativeImageTextFileUpload(params);
	return _createJavascriptImageTextFileUpload(params);
};*/