#pragma once
#include "SystemChecksResult.hpp"
#include <memory>
class SystemChecks{
	public:
		static std::shared_ptr<SystemChecksResult> run();
};