#include "SystemChecksResult.hpp"
SystemChecksResult::getSuccess(){
	return _success;
}
SystemChecksResult::getErrorMessage(){
	return _errorMessage;
}
SystemChecksResult::SystemChecksResult(bool success, std::string errorMessage):
	_success(success),
	_errorMessage(errorMessage){
		
}