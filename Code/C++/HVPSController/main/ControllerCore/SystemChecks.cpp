#pragma once
#include "SystemChecks.hpp"
SystemChecksResult SystemChecks::run(){
	return std::make_shared<SystemChecksResult>(true);
}