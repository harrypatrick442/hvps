var WindowResizeManager=new (function(){
		EventEnabledBuilder(this);
		var self = this;
		/*var temporalCallback = new TemporalCallback({callback:doResize, delay:500,
		maxTotalDelay:800});*/
		function scheduleResize(){
			//temporalCallback.trigger();
		}
		function doResize(){
			dispatchResized();
		}
		function dispatchResized(){
			self.dispatchEvent({'type':'resized'});
		}
})();