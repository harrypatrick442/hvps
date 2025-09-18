import eventEnable from '../core/eventEnable';
import ParameterErrors from '../errors/ParameterErrors';
const{throwNotProvided}=ParameterErrors;
export default class TabInfo{
	constructor(props){
		const {title, className}=props;
		if(title===undefined||title===null)throwNotProvided('title');
		this._title = title;
		this._className = className;
	}
	get title(){
		return this._title;
	}
	get className(){
		return this._className;
	}
}