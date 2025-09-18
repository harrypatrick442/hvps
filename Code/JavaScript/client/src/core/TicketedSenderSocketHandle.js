import GeneratedConstants from '../constants/Generated';
const DEFAULT_TIMEOUT=120000;
export default function TicketedSenderSocketHandle(socket, callbackDispose){
	var mapTicketToCallbackAndTimeSent={};
	socket.addEventListener('message', onMessage);
	this.addTicketedCallback=function(callback, ticket, timeoutMs, callbackTimeout){
		mapTicketToCallbackAndTimeSent[ticket]={
			timeSent:getTime(),
			callback:callback,
			customTimeout:timeoutMs,
			callbackTimeout:callbackTimeout
		};
	};
	this.doTimeouts=function(now){
		for(var ticket in mapTicketToCallbackAndTimeSent){
			var callbackAndTimeSent = mapTicketToCallbackAndTimeSent[ticket];
			var timeoutMs = callbackAndTimeSent.customTimeout;
			var timeoutIfOlderThan = timeoutMs?(now - timeoutMs):now - DEFAULT_TIMEOUT;
			if(callbackAndTimeSent.timeSent<timeoutIfOlderThan)
			{
				var callbackTimeout=callbackAndTimeSent.callbackTimeout;
				if(callbackTimeout){
					try{
						callbackTimeout();
					}
					catch(ex){
						console.error(ex);
					}
				}
				delete mapTicketToCallbackAndTimeSent[ticket];
			}
		}
		disposeIfHasNoPending();
	};
	function onMessage(e){
		var message = e.message;
		var ticket = message[GeneratedConstants.Ticketing.TICKET];
		if(!ticket){
			return;
		}
		doCallbackForTicket(ticket, message);
	}
	function hasTicketsPending(){
		for(var ticket in mapTicketToCallbackAndTimeSent) {
			if (mapTicketToCallbackAndTimeSent.hasOwnProperty(ticket)) {
				return true;
			}
		}
		return false;
	}
	function disposeIfHasNoPending(){
		if(!hasTicketsPending()){
			dispose();
		}
	}
	function doCallbackForTicket(ticket, msg){
		var callbackAndTimeSent = mapTicketToCallbackAndTimeSent[ticket];
		if(!callbackAndTimeSent){
			return;
		}
		callbackAndTimeSent.callback(msg);
		delete mapTicketToCallbackAndTimeSent[ticket];
		disposeIfHasNoPending();
	}
	function dispose(){
		socket.removeEventListener('message', onMessage);
		callbackDispose();
	}
	function getTime(){
		return new Date().getTime();
	}
}