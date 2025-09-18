import eventEnable from '../core/eventEnable';
export default class CompositeProgressSource 
{
	constructor(props){
		const {weightingFactor, progressSources}=props||{};
		eventEnable(this);
		
		this.add= this.add.bind(this);
		this._add= this._add.bind(this);
		this.clear= this.clear.bind(this);
		this.remove= this.remove.bind(this);
		this._onProgressSourceProgressChanged= this._onProgressSourceProgressChanged.bind(this);
		this._update= this._update.bind(this);
		this.done= this.done.bind(this);
		this._dispatchProgressChanged= this._dispatchProgressChanged.bind(this);
		this._sum= this._sum.bind(this);
		
		this._proportion = 0;
		this._isDone = false;
		this._weightingFactor = weightingFactor===undefined?null:weightingFactor;
		this._progressSources =[];
		
		if(progressSources!==undefined&&progressSources!==null){
			this.add(progressSources);
		}
	}
	get progressProportion(){
		return this._isDone?1: this._proportion; 
	}
	get weightingFactor()
	{
		if (this._weightingFactor !== null) return this._weightingFactor;
		var weightingFactorSum=0;
		this._progressSources.forEach(progressSource => weightingFactorSum +=progressSource.weightingFactor);
		return weightingFactorSum;
	}
	add(progressSource_s)
	{
		if(Array.isArray(progressSource_s)){
			if (progressSource_s.length < 1) return;
		}
		else{
			progressSource_s=[progressSource_s];
		}
		var added = false;
			progressSource_s.forEach (progressSource => added |= this._add(progressSource));
		if (added)
			this._update();
	}
	clear()
	{
		this._progressSources.forEach(progressSource=>this.remove(progressSource));
		this._isDone = false;
		this._update();
	}
	remove(progressSource)
	{
		const index = this._progressSources.indexOf(progressSource);
		if(index<0)return;
		progressSource.removeEventListener('progresschanged', this._onProgressSourceProgressChanged);
		this._progressSources.splice(index, 1);
	}
	done()
	{ 
		this._isDone = true;
		this._update();
	}
	_add(progressSource)
	{
		if (this._progressSources.indexOf(progressSource)>=0) return false;
		this._progressSources.push(progressSource);
		progressSource.addEventListener('progresschanged', this._onProgressSourceProgressChanged);
		return true;
	}
	_onProgressSourceProgressChanged(e)
	{
		this._update();
	}
	_update()
	{
		const oldProportion = this._proportion;
		if (this._sum() !== oldProportion)
			this._dispatchProgressChanged();
	}
	_dispatchProgressChanged()
	{
		this.dispatchEvent({type:'progresschanged', progressProportion:this.progressProportion});
	}
	_sum()
	{
		if (this._isDone) return 1;
		const count = this._progressSources.length;
		if (count < 1)
		{
			this._proportion = 0;
			return 0;
		}
		let sumProgressTimesWeight = 0;
		let sumWeights = 0;
		this._progressSources.forEach(progressSource=>{
			sumProgressTimesWeight += progressSource.weightingFactor * (
				progressSource.progressProportion > 1 ? 1 : (
					progressSource.progressProportion < 0 ? 0 : progressSource.progressProportion
				)
			);
			sumWeights += progressSource.weightingFactor;
		});
		return (this._proportion = (sumProgressTimesWeight / sumWeights));
	}
}