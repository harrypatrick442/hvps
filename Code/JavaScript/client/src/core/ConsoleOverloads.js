export default class ConsoleOverloads {
	static overloadLog(method) {
		const _privateLog = console.log;
		console.log = function (message) {
			_privateLog.apply(console, arguments);
			try{
				method.apply(null, arguments);
			}catch{}
		};
	}
	static overloadError(method) {
		const _privateError = console.error;
		console.error = function (message) {
			_privateError.apply(console, arguments);
			try{
				method.apply(null, arguments);
			}catch{}
		};
	}
}