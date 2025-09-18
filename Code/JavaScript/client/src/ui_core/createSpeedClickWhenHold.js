import EfficientMoveCycle from './EfficientMoveCycle';
export default function createSpeedClickWhenHold({
	element, 
	delayTillStartMilliseconds, 
	minFrequencyInclusive, 
	maxFrequencyInclusive, 
	delayFromMinToMaxFrequencyMilliseconds,
	callback,
	disposes
}){
	const frequencyDifference = maxFrequencyInclusive - minFrequencyInclusive;
	let currentDelay, startedAt, doClick, proportionFrequencyDifference,timeout;
	let done = false;
	doClick = ()=>{
		callback();
		if(done){
			return;
		}
		if(proportionFrequencyDifference>1){
			timeout = setTimeout(doClick, currentDelay);
			return;
		}
		let newProportion = ((new Date().getTime() - startedAt)/delayFromMinToMaxFrequencyMilliseconds);
		if(newProportion>1){
			newProportion = 1;
		}
		proportionFrequencyDifference = newProportion;
		currentDelay = 1000/((proportionFrequencyDifference*frequencyDifference)+minFrequencyInclusive);
		timeout = setTimeout(doClick, currentDelay);
	};
	const onMouseDown =  ()=>{
		done = false;
		startedAt = new Date().getTime();
		currentDelay= (1000/minFrequencyInclusive);
		timeout = setTimeout(()=>{
			if(done)return;
			proportionFrequencyDifference = 0;
			callback();
			timeout = setTimeout(doClick, currentDelay);
		}, delayTillStartMilliseconds);
	};
	const onMouseUp = ()=>{
		clearTimeout(timeout);
		done = true;
	};
	const efficientMoveCycle = new EfficientMoveCycle({element, onStart:onMouseDown, onMove:()=>{}, onEnd:onMouseUp});
	
	disposes.push(efficientMoveCycle.dispose);
	disposes.push(()=>{
		done = true;
	});
}