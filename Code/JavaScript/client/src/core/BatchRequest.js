export default class BatchRequest {
    constructor() {
        this._inFlightRequest = null;
        this.run = this.run.bind(this);
    }
    run(fn) {
        const self = this;
        if (self._inFlightRequest != null) return self._inFlightRequest;
        self._inFlightRequest = fn();
        self._inFlightRequest.then(function () { self._inFlightRequest = null; }, 
            function () { self._inFlightRequest = null; }); 
        return this._inFlightRequest;
    }
}