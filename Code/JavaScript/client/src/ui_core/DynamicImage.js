import DynamicImageEntry from './DynamicImageEntry';
import ParameterErrors from '../errors/ParameterErrors';
const { throwNotProvided, throwNotOfType } = ParameterErrors;
export default class DynamicImage{
	constructor(props){
		const{entries}=props;
		if(entries===undefined||entries===null)throwNotProvided('entries');
		this._entries = entries;
	}
	static fromJSON(jObject){
		const entries = jObject.entries?.map(entry=>DynamicImageEntry.fromJSON(entry));
		return new DynamicImage({entries:entries});
	}
}