#include "UARTBase.hpp"
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
_invertRx(invertRx){
}
UARTBase::~UARTBase() {
	std::unique_lock<std::mutex> lock(_mutexClaimReleaseNUart);
	if (_claimedUartPort) {
		_usedUarts.erase(_nUART);
	}
}
int UARTBase::getNUART() const{
	return _nUART;
}
bool UARTBase::checkNUARTValid(int nUART){
	std::unique_lock<std::mutex> lock(_mutexClaimReleaseNUart);
	if (_usedUarts.contains(nUART)) {
		Aborter::safeAbort(TAG, "UART%d is already in use", nUART);
		return false;
	}
	_usedUarts.insert(nUART);
	_claimedUartPort = true;
	return true;
}