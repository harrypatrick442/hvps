export default class Handle{
	constructor(props){
		this._callbackDispose = props.dispose;
		this.dispose = this.dispose.bind(this);
	}
	dispose(){
		this._callbackDispose(this);
	}
}