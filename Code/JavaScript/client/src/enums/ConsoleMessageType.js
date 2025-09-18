const ConsoleMessageType = {
	Info:0,
	Error:1,
};
ConsoleMessageType.getCssClass = (value)=>{
	switch(value){
		case ConsoleMessageType.Error:
			return 'error';
		case ConsoleMessageType.Info:
		default:
			return 'info';
	}
};
export default ConsoleMessageType;