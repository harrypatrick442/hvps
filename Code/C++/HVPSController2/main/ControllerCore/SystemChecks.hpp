#pragma once
#include "SystemChecksResult.hpp"
#include "../Ports/Port_VoltageFeedbackBase.hpp"
#include <functional>
#include <string> 
class SystemChecks{
	public:
		static std::shared_ptr<SystemChecksResult> run();
		static bool run(std::string& errorMessage);
	private:
	/*
		*/
};