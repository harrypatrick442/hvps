export default function stopEventPropagation(element, eventList) {
	if(!eventList)
		eventList= [ 'touchstart', 'touchmove', 'touchend', 'touchcancel','wheel', 'mousewheel' ];
	for (var i = 0; i < eventList.length; i++) {
		element.addEventListener(eventList[i], function(event) {
			event.stopPropagation();
		});
	}
}