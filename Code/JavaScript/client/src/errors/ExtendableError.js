export default class ExtendableError extends Error {
    constructor(message, innerError) {
		if(innerError)
			console.error(innerError);
        super(message);
        this.name = this.constructor.name;
        this.innerError = innerError;
        if (typeof Error.captureStackTrace === 'function') {
            Error.captureStackTrace(this, this.constructor);
        } else {
            this.stack = (new Error(message)).stack;
        }
    }
}
