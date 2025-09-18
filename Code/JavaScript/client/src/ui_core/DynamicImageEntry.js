export default class DynamicImageEntry{
	constructor(props){
		const{relativePath, width, height}=props;
		if(relativePath===undefined||relativePath===null)throwNotProvided('relativePath');
		this._relativePath = relativePath;
		this._width = width;
		this._height = height;
	}
	get width(){
		return this._width;
	}
	get height(){
		return this._height;
	}
}