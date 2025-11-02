#include "ChannelEvents.hpp"
EventConnection ChannelEvents::addOnOpenedHandler(
	std::function<void(const ChannelEventArgs&)> handler){
			return onOpened.addHandler(std::move(handler));
}
EventConnection ChannelEvents::addOnClosedHandler(
	std::function<void(const ChannelEventArgs&)> handler){
			return onClosed.addHandler(std::move(handler));
	
}
	
void ChannelEvents::dispatchOnOpened(){
	_worker->enqueue([this]{ 
		ChannelEventArgs e; 
		onOpened.dispatch(e); 
	});
}
void ChannelEvents::dispatchOnClosed(){
	_worker->enqueue([this]{ 
		ChannelEventArgs e; 
		onClosed.dispatch(e); 
	});
}