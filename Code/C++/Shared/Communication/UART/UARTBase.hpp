#pragma once
#include <unordered_set> 
#include <mutex>
#include "../Interfaces/IChannel.hpp"
class UARTBase: public IChannel{
public:
	inline static constexpr size_t MIN_REQUIRED_RECEIVE_BUFFER_SIZE = 256;
private:
	static std::mutex _mutexClaimReleaseNUart;	
	static std::unordered_set<int> _usedUarts;
	bool _claimedUartPort;
protected:
	int _nUART;
	int _txPin; 
	int _rxPin;
	int _baudRate;
	bool _invertTx;
	bool _invertRx;
    char _description[16]; // enough for "UART123" etc.
public:
	UARTBase(
		int nUART,
		int txPin, 
		int rxPin, 
		int baudRate,
		bool invertTx, 
		bool invertRx);
	virtual ~UARTBase();
	const char* getDescription() const;
protected:
	bool checkNUARTValid(int nUART);
public:
	virtual bool configure() = 0;
	virtual size_t readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) = 0;
	virtual size_t writeBytes(const char* src, size_t len) = 0;
	virtual void flushTx() = 0;
};