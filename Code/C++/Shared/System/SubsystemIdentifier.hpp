#pragma once
class SubsystemIdentifier{
private:
	static int _value;
public:
	static void set(int value);
	static int get();
};