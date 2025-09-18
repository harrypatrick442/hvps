import eventEnable from '../core/eventEnable';
export default class StandardProgressSource
{
	constructor(props){
		const {weightingFactor}=props||{};
		eventEnable(this);
		this.done= this.done.bind(this);
		this._dispatchProgressChanged= this._dispatchProgressChanged.bind(this);
		this._weightingFactor = weightingFactor===undefined||weightingFactor===null?1:weightingFactor;
		this._isDone = false;
		this._proportion = 0;
	}
	get progressProportion (){ 
		return this._isDone ? 1 : this._proportion;
	}
	set progressProportion(value){
		if(this._proportion===value)return;
		this._proportion= value;
		this._dispatchProgressChanged();
	}
	get weightingFactor()
	{
		return this._weightingFactor;
	}
	done() {
		if(this._isDone)return;
		this._isDone = true; 
		this._dispatchProgressChanged(); 
	}
	_dispatchProgressChanged()
	{
		this.dispatchEvent({type:'progresschanged', progressProportion:this.progressProportion});
	}
}