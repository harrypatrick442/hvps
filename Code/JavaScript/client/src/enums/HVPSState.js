import isNullOrUndefined from '../core/isNullOrUndefined';
const HVPSState={
	Live:1,
	StandbyLive:2,
	StandbySafe:3,
	Shutdown:4,
	Unknown:0
};
HVPSState.getClassName = (value)=>{
	if(isNullOrUndefined(value)){
		return value;
	}
	switch(value){
		case HVPSState.Live:
			return 'live';
		case HVPSState.StandbyLive:
			return 'standby-live';
		case HVPSState.StandbySafe:
			return 'standby-safe';
		case HVPSState.Shutdown:
			return 'shutdown';
		case HVPSState.Unknown:
			return 'unknown';
		default:
			throw new Error(`Not implemented for value ${value}`);
	}
};
HVPSState.getDescription = (value)=>{
	if(isNullOrUndefined(value)){
		return value;
	}
	switch(value){
		case HVPSState.Live:
			return 'Live';
		case HVPSState.StandbyLive:
			return 'Standby Live';
		case HVPSState.StandbySafe:
			return 'Standby Safe';
		case HVPSState.Shutdown:
			return 'Shutdown';
		case HVPSState.Unknown:
			return 'Unknown';
		default:
			throw new Error(`Not implemented for value ${value}`);
	}
};
export default HVPSState;