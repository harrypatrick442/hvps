#include "TicketedSenderHandle.hpp"
#include "../Core/UsageGuard.hpp"
#include <mutex>
//Returns true if should dispose responseMessage
bool TicketedSenderHandle::complete(cJSON* responseMessage){
	std::unique_lock<std::mutex> lock(_mutex);
	/*We require a mutex to be sure if latch was already 
	unlatched and cleanup the responseMessage if its already 
	unlatched.. And also avoid replacing it if we get two incoming messages.
	We do not actually require the mutex to set and read _responseMessage. Its to
	prevent leaking cJSON*'s by allowing a safe isLatched check.
	*/
	if(!_latchWait.isLatched()){
		return true;
	}
	_responseMessage = responseMessage;
	_latchWait.unlatch();
	return false;
}
void TicketedSenderHandle::fail(){
	std::unique_lock<std::mutex> lock(_mutex);
	_latchWait.unlatch();
}
cJSON* TicketedSenderHandle::wait(){
	_latchWait.wait();
	return _responseMessage;
}
cJSON* TicketedSenderHandle::wait(CancellationToken* cancellationToken){
	if(cancellationToken==nullptr){
		_latchWait.wait();
		return _responseMessage;
	}
    UsageGuard<const CancellationToken> cancellationTokenUsageGuard(cancellationToken);
	int regHandle = cancellationToken->reg([this]{
		std::unique_lock<std::mutex> lock(_mutex);
        _latchWait.unlatch();
    });

    _latchWait.wait(); // will return either on complete() or on cancel()

    if (regHandle >= 0) {
        cancellationToken->deregister(regHandle);
    }
	return _responseMessage;
}
bool TicketedSenderHandle::checkTimedOut(uint64_t millisecondsNow){
	if (millisecondsNow < _timeoutAtThresholdMilliseconds) 
	{
		return false;
	}
	fail();
	return true;
}
TicketedSenderHandle::TicketedSenderHandle(uint64_t timeoutAtThresholdMilliseconds):
	_timeoutAtThresholdMilliseconds(timeoutAtThresholdMilliseconds),
	_responseMessage(nullptr)
{
	
}
