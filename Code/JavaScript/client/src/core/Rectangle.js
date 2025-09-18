export default class Rectangle{
	constructor({l, t, w, h}){
		if(isNullOrUndefined(l))
			throw new Error('l');
		if(isNullOrUndefined(t))
			throw new Error('t');
		if(isNullOrUndefined(w))
			throw new Error('w');
		if(isNullOrUndefined(h))
			throw new Error('h');
		this._l = l;
		this._t = t;
		this._w = w;
		this._h = h;
	}
	get l(){
		return this._l;
	}
	get t(){
		return this._t;
	}
	get w(){
		return this._w;
	}
	get h(){
		return this._h;
	}
}