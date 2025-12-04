#pragma once
#include <unordered_set> 
#include <mutex>
#include "../Interfaces/IChannelBase.hpp"
class UARTBase: public IChannelBase{
public:
	inline static constexpr const char* TAG= "UARTBase";
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
	virtual int readBytes(char* dst, size_t maxlen, uint32_t timeoutMs) = 0;
	virtual int writeBytes(const char* src, size_t len) = 0;
	virtual void flushTx() = 0;
};