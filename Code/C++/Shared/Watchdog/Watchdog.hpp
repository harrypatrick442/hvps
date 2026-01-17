#pragma once
class Watchdog{
private:
	volatile uint32_t _countFrom;
	volatile uint32_t _count;
	const char* _name;
public:
	void run();
	bool check();
	const char* getName() const;
};