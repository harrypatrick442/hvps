#include "SystemChecksResult.hpp"
bool SystemChecksResult::getSuccess(){
	return _success;
}
std::string SystemChecksResult::getErrorMessage(){
	return _errorMessage;
}
SystemChecksResult::SystemChecksResult(bool success, std::string errorMessage):
	_success(success),
	_errorMessage(errorMessage){
		
}