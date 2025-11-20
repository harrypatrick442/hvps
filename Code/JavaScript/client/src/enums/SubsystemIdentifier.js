const SubsystemIdentifier = {
	HVPSController:1,
	FirstStageVoltageFeedbackModule:2,
	OutputVoltageFeedbackModule:3
};
SubsystemIdentifier.getDescription = (value)=>{
	switch(value){
		case SubsystemIdentifier.HVPSController:
			return 'HVPSController';
		case SubsystemIdentifier.FirstStageVoltageFeedbackModule:
			return 'FirstStageVoltageFeedbackModule';
		case SubsystemIdentifier.OutputVoltageFeedbackModule:
			return 'OutputVoltageFeedbackModule';
		default:
			return `Unknown subsystem with identifier ${value}`;
		
	}
};
export default SubsystemIdentifier;