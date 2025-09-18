import ParameterErrors from '../errors/ParameterErrors';
const { throwNotProvided, throwNotOfType} = ParameterErrors;
export default class TwoFingerTouch{
	constructor(props){
		const {element, onStart, onStartFinger1, onStartFinger2, onMoveFinger1, onMoveFinger2, onEndFinger1, onEndFinger2, onEnd}=props;
		this._finger1Active = false;
		this._finger2Active=false;
		this._touch1 = null;
		this._touch2 = null;
		this._onStart=onStart;
		this._onMoveFinger2 = onMoveFinger2;
		this._onMoveFinger1 = onMoveFinger1;
		this._onStartFinger1 = onStartFinger1;
		this._onStartFinger2 = onStartFinger2;
		this._onEndFinger1 = onEndFinger1;
		this._onEndFinger2 = onEndFinger2;
		this._onEnd = onEnd;
		this._finger1Id = null;
		this._finger2Id = null;
		
		this._start= this._start.bind(this);
		this._startAnywhere= this._startAnywhere.bind(this);
		this._startFinger2= this._startFinger2.bind(this);
		this._move= this._move.bind(this);
		this._end= this._end.bind(this);
		//onStartFinger1-2
		//onMoveFinger1-2
		//onEndFinger1-2
		//onStart when both fingers are down
		//onEnd when both fingers are up.
		element.addEventListener('touchstart', this._start);
	}
	_start(e){
			var changedTouches = e.changedTouches;
			for(var i=0; i<changedTouches.length; i++){
				var changedTouch = changedTouches[i];
				const identifier = changedTouch['identifier'];
				if(!this._finger1Active){
					this._finger1Active=true;
					this._finger1Id=identifier;
					setTimeout(function(){
						document.documentElement.addEventListener('touchstart', this._startAnywhere);
					}, 0);
					document.documentElement.addEventListener('touchmove', this._move);
					document.documentElement.addEventListener('touchend', this._end);
					this._touch1 = changedTouch;
					this._onStartFinger1&&this._onStartFinger1(changedTouch,  e);
				}
				else
					this._startFinger2(changedTouch, e);
			}
	}
	
	_startAnywhere(e){
		var changedTouch = e.changedTouches[0];
		this._startFinger2(changedTouch, e);
	}
	_startFinger2(changedTouch, e){
		if(this._finger1Active&&!this._finger2Active){
			this._finger2Active=true;
			this._finger2Id = changedTouch['identifier'];
			this._touch2 = changedTouch;
			this._onStartFinger2&&this._onStartFinger2(changedTouch, e);
			if(this._finger1Active&&this._finger2Active)
			{
				this._onStart&&this._onStart({
					touch1:this._touch1,
					touch2:this._touch2,
					e:e
				});
			}
		}
	}
	
	_move(e){
		var changedTouches = e.changedTouches;
		for(var i=0; i<changedTouches.length; i++){
			var changedTouch = changedTouches[i];
			if(changedTouch.identifier ===this._finger1Id){
				this._onMoveFinger1&&this._onMoveFinger1(changedTouch,  e);
			}
			if(changedTouch.identifier==this._finger2Id){
				this._onMoveFinger2&&this._onMoveFinger2(changedTouch, e);
			}
		}
		try{
		e.preventDefault&&e.preventDefault();
		}
		catch(err){}
	}
	_end(e){
			var changedTouches = e.changedTouches;
			for(var i=0; i<changedTouches.length; i++){
				var changedTouch = changedTouches[i];
				const identifier = changedTouch['identifier'];
				if(identifier===this._finger1Id){
					this._finger1Active=false;
					document.documentElement.removeEventListener('touchstart', this._startAnywhere);
					this._onEndFinger1&&this._onEndFinger1(changedTouch,  e);
					continue;
				}
				if(identifier===this._finger2Id){
					this._finger2Active=false;
					this._onEndFinger2&&this._onEndFinger2(changedTouch, e);
					continue;
				}
				this._finger2Active=false;
				this._finger1Active=false;
			}
			var active = this._finger1Active||this._finger2Active;
			if(active)return true;
			document.documentElement.removeEventListener('touchmove', this._move);
			document.documentElement.removeEventListener('touchend', this._end);
			this._onEnd&&this._onEnd(e);
	}
}