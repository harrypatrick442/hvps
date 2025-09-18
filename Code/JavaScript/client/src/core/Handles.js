import eventEnable from './eventEnable';
import Handle from './Handle';
export default class Handles{
	constructor(props){
		this._onChange = props&&props.onChange;
		eventEnable(this);
		this._handles = [];
		this._currentlyHasHandles = false;
		this.take = this.take.bind(this);
		this._dispose = this._dispose.bind(this);
		this._dispatchChangedIfChanged = this._dispatchChangedIfChanged.bind(this);
		this._dispatchChanged = this._dispatchChanged.bind(this);
	}
	take(){
		const handle = new Handle({dispose:this._dispose});
		this._handles.push(handle);
		this._dispatchChangedIfChanged();
		return handle;
	}
	get taken(){
		return this._handles.length>0;
	}
	_dispose(handle){
		const handles = this._handles;
		const index = handles.indexOf(handle);
		if(index<0)return;
		handles.splice(index, 1);
		this._dispatchChangedIfChanged();
	}
	_dispatchChangedIfChanged(){
		if((this._handles.length>0)^(this._currentlyHasHandles))
		{
			this._currentlyHasHandles=!this._currentlyHasHandles;
			this._dispatchChanged();
		}
	}
	_dispatchChanged(){
		this._onChange&&this._onChange(this._currentlyHasHandles);
		this.dispatchEvent({type:'change', hasHandles:this._currentlyHasHandles});
	}
}