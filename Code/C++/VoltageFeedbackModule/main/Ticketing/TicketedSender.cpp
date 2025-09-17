#include "TicketedSender.hpp"
#include "TicketSource.hpp"
#include "../Timing/Delay.hpp"
#include "../Timing/TimeHelper.hpp"
#include "../Messaging/MessageConstants.hpp"
#include "../JSON/JHelper.hpp"
#include "../Logging/Log.hpp"
#include <iostream>
#include <cstring>
const char* TicketedSender::TAG="TicketedSender";
Timer TicketedSender::_timerCheckTimedOutAllInstances = 
	Timer(50, TicketedSender::checkTimedOutOnAllInstances, true/*repeat*/);
std::unordered_set<TicketedSender*> TicketedSender::_instances;
std::mutex TicketedSender::_mutexInstances;
TicketedSender::TicketedSender(CallbackSend callbackSend):
_disposed(false), _callbackSend(callbackSend){
	std::unique_lock<std::mutex> lock(_mutexInstances);
	_instances.insert(this);
    _timerCheckTimedOutAllInstances.start();
}
TicketedSender::~TicketedSender(){
	markDisposedAndCleanupHandles();
	removeFromInstances(this);
}
void TicketedSender::markDisposedAndCleanupHandles(){
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_disposed = true;
		if (_mapTicketToHandle.empty()) return;
		for (auto it = _mapTicketToHandle.begin(); it != _mapTicketToHandle.end(); ++it) {
			TicketedSenderHandle* handle = it->second;
			handle->fail(); // may remove itself from the map
		}
	}
	
	while(true){
		/*Wait for all handles to be gone. This means no further pending on them and its safe to delete this.*/
		{
			std::unique_lock<std::mutex> lock(_mutex);
			if(_mapTicketToHandle.empty()){
				return;
			}
		}
		Delay::ms(10);
	}
}
void TicketedSender::removeFromInstances(TicketedSender* ticketedSender){
	_mutexInstances.lock();
	_instances.erase(ticketedSender);
	_mutexInstances.unlock();
}
cJSON* TicketedSender::send(cJSON* request, uint64_t timeoutMilliseconds, CancellationToken* cancellationToken){	
	uint64_t ticket = TicketSource::next();
	Log::Info(TAG, "Ticket sent was %" PRIu64 "", ticket);
	JHelper::setUInt64(request, MessageConstants::TICKET_KEY, ticket);
	Log::Info(TAG, "Modified ticketed message to send key value pairs");
	JHelper::printJsonKeysAndValues(request);
	TicketedSenderHandle* handle = new TicketedSenderHandle(
		TimeHelper::getTimeMilliseconds() + timeoutMilliseconds);
	std::unique_lock<std::mutex> lock(_mutex);
	if(_disposed){
		delete handle;
		return nullptr;
	}
	_mapTicketToHandle[ticket] = handle;
	lock.unlock();
	_callbackSend(request);
	cJSON* responseMessage = handle->wait(cancellationToken);
	removeHandle(handle, ticket);
	return responseMessage;
}
void TicketedSender::removeHandle(TicketedSenderHandle* handle, uint64_t ticket)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_mapTicketToHandle.erase(ticket);
	delete handle;
}
bool TicketedSender::handleTicketedMessage(cJSON* message){
	bool success = true;
	char* type = JHelper::getString(message, MessageConstants::TYPE_KEY, success);
	if(!success){
		return false;
	}
	return handleTicketedMessage(message, type);
}
bool TicketedSender::handleTicketedMessage(cJSON* message, char* type){
	if(strcmp(type, MessageConstants::TYPE_TICKETED_VALUE) != 0){
		return false;
	}
	bool success = true;
	uint64_t ticket = JHelper::getUInt64(message, MessageConstants::TICKET_KEY, success);
	if(!success){
		return false;
	}
	if(ticket==0){
		Log::Error(TAG, "Got 0 ticket!");
		return false;
	}
	
	//Log::Info(TAG, "Ticket received was %" PRIu64 "", ticket);
	std::unique_lock<std::mutex> lock(_mutex);
	/*Log::Info(TAG, "Map had %d", _mapTicketToHandle.size());
	for (const auto& [t, handlePtr] : _mapTicketToHandle) {
		Log::Info(TAG, "map ticket  %" PRIu64 "", t);
	}*/
	auto it = _mapTicketToHandle.find(ticket);
	if (it == _mapTicketToHandle.end()) {
		return false;
	}
	TicketedSenderHandle* handle = it->second;
	lock.unlock();
	if(handle->complete(message)){
		cJSON_Delete(message);
	}
	return true;
}
void TicketedSender::checkTimedOutOnAllInstances()
{
	
    const uint64_t millisecondsNow = TimeHelper::getTimeMilliseconds();
	std::unique_lock<std::mutex> lock(_mutexInstances);
	int instancesLength = _instances.size();
	if(instancesLength<1){
		_timerCheckTimedOutAllInstances.stop();
		return;
	}
	std::vector<TicketedSender*> instancesSnapshot;
	instancesSnapshot.reserve(instancesLength);
	instancesSnapshot.insert(instancesSnapshot.end(), _instances.begin(), _instances.end());
	lock.unlock();
	for (auto* instance : instancesSnapshot) {
		instance->checkTimedOuts(millisecondsNow);
	}
}
void TicketedSender::checkTimedOuts(uint64_t millisecondsNow) {
	std::unique_lock<std::mutex> lock(_mutex);
	if (_mapTicketToHandle.empty()) return;
	for (auto it = _mapTicketToHandle.begin(); it != _mapTicketToHandle.end(); ++it) {
		TicketedSenderHandle* handle = it->second;
        handle->checkTimedOut(millisecondsNow);
	}
}

