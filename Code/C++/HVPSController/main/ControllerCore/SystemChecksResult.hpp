#pragma once
class SystemChecksResult{
private:
	bool _success;
	std::string _errorMessage;
public:
	getSuccess();
	getErrorMessage();
	
	SystemChecksResult(bool success, std::string errorMessage):
		_success(success),
		_errorMessage(errorMessage){
		
	}
};