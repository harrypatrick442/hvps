#pragma once
class SubsystemIdentifier{
private:
	inline static constexpr const char* TAG = "SubsystemIdentifier";
	static int _value;
public:
	static void set(int value);
	static int get();
};