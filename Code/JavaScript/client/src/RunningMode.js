export default class RunningMode{
	static get isDevelopment(){
		return process.env.NODE_ENV === 'development';
	}
}
window.RunningMode = RunningMode;
