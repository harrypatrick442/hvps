import ParameterErrors from '../errors/ParameterErrors';
const { throwNotProvided, throwNotOfType} = ParameterErrors;
export default class InsertedMnuItemsRegistrationHandle{
	constructor(props){
		const {callbackRemove}=props;
		if(!callbackRemove)throwNotProvided('callbackRemove');
		this._callbackRemove = callbackRemove;
		this._disposed = false;
		this.dispose= this.dispose.bind(this);
	}
	dispose(){
		if(this._disposed)return;
		this._disposed = true;
		this._callbackRemove();
	}
}