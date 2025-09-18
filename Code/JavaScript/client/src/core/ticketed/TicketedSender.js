import TicketedSenderSocketHandle from './TicketedSenderSocketHandle';
import Timer from './Timer';
import HashBuilder from './HashBuilder';
import GeneratedConstants from ',,/constants/Generated';
let _nTicket=0;
const _mapSocketHashToTicketedSenderSocketHandle = {};
export default class TicketedSender {
	static send(params){
		return new Promise((resolve, reject)=>{
			const {socket, msg, timeoutMs}=params;
			if(!socket)throw new Error('No socket');
			var mysocketHash = HashBuilder(socket);
			var handle = _mapSocketHashToTicketedSenderSocketHandle[mysocketHash];
			if(!handle){
				var handle = new TicketedSenderSocketHandle(socket, ()=>{
					delete _mapSocketHashToTicketedSenderSocketHandle[socket.hash];
				});
				_mapSocketHashToTicketedSenderSocketHandle[mysocketHash]=handle;
			}
			var ticket = TicketedSender._generateTicket();
			handle.addTicketedCallback((res)=>{
				resolve(res);
			}, ticket, timeoutMs, ()=>{
				reject(new Error('Timed out'));
			});
			msg[GeneratedConstants.Ticketing.TICKET]=ticket;
			socket.send(msg);
		});
	};
	static _doTimeouts(){
		const now = TicketedSender._getTime();
		for(var socketHash in _mapSocketHashToTicketedSenderSocketHandle){
			var handle = _mapSocketHashToTicketedSenderSocketHandle[socketHash];
			handle.doTimeouts(now);
		}
	}
	static _getTime(){
		return new Date().getTime();
	}
	static _generateTicket(){
		return 'ticket_'+String(TicketedSender._getTime())+'_'+String(_nTicket++);
	};
}
var timerTimeout = new Timer({nTicks:-1, delay:1000, callback:TicketedSender._doTimeouts});
timerTimeout.start();