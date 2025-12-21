#include "UARTBase.hpp"
#include "System/SafeAbort.hpp"
std::mutex UARTBase::_mutexClaimReleaseNUart;
std::unordered_set<int> UARTBase::_usedUarts;
UARTBase::UARTBase(
		int nUART,
		int txPin, 
		int rxPin, 
		int baudRate,
		bool invertTx, 
		bool invertRx):
_claimedUartPort(false),
_nUART(nUART),
_txPin(txPin),
_rxPin(rxPin),
_baudRate(baudRate),
_invertTx(invertTx),
_invertRx(invertRx)
{
    std::snprintf(_description, sizeof(_description), "UART%d", nUART);
}
UARTBase::~UARTBase() {
	std::unique_lock<std::mutex> lock(_mutexClaimReleaseNUart);
	if (_claimedUartPort) {
		_usedUarts.erase(_nUART);
	}
}
const char* UARTBase::getDescription() const{
	return _description;
}
bool UARTBase::checkNUARTValid(int nUART){
	std::unique_lock<std::mutex> lock(_mutexClaimReleaseNUart);
	if (_usedUarts.contains(nUART)) {
		SAFE_ABORT("UART%d is already in use", nUART);
		return false;
	}
	_usedUarts.insert(nUART);
	_claimedUartPort = true;
	return true;
}