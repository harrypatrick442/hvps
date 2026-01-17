#pragma once
#include <cstdint>
class Watchdog{
private:
	volatile uint32_t _countFrom;
	volatile uint32_t _count;
	const char* _name;
public:
	Watchdog(const char* name, uint32_t countFrom);
	void run();
	bool check();
	const char* getName() const;
};