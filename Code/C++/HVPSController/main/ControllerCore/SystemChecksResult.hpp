#pragma once
#include <string>
class SystemChecksResult{
private:
	bool _success;
	std::string _errorMessage;
public:
	bool getSuccess();
	std::string getErrorMessage();
	
	SystemChecksResult(bool success, std::string errorMessage);
};