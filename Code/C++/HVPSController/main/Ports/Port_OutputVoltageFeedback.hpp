#pragma once
#include "Port_VoltageFeedbackBase.hpp"
#include "Core/SingletonBase.hpp"
class Port_OutputVoltageFeedback final :
	public Port_VoltageFeedbackBase,
	public SingletonBase<Port_OutputVoltageFeedback>{
    public:
		static const char* getTag();
		static Port_OutputVoltageFeedback& initialize() noexcept;
    private:
		friend class SingletonBase<Port_OutputVoltageFeedback>; // lets SingletonBase call our ctor
		explicit Port_OutputVoltageFeedback(TOSLINKDuplexChannel* ch) noexcept;
};