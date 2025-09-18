import eventEnable from '../core/eventEnable';
export default BinaryProgressSource
{
	constructor(props){
		const {weightingFactor}=props||{};
		eventEnable(this);
		this.done= this.done.bind(this);
		this._dispatchProgressChanged= this._dispatchProgressChanged.bind(this);
		this._weightingFactor = weightingFactor===undefined||weightingFactor===null?1:weightingFactor;
		this._isDone = false;
	}
	get progressProportion(){
		return this._isDone?1: 0; 
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