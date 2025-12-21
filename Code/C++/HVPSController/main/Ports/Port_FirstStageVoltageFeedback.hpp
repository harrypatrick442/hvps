#pragma once
#include "Port_VoltageFeedbackBase.hpp"
#include "Core/SingletonBase.hpp"
class Port_FirstStageVoltageFeedback final :
	public Port_VoltageFeedbackBase,
	public SingletonBase<Port_FirstStageVoltageFeedback>{
    public :
		static const char* getTag();
		static Port_FirstStageVoltageFeedback& initialize() noexcept;
    private:
		friend class SingletonBase<Port_FirstStageVoltageFeedback>; // lets SingletonBase call our ctor
		explicit Port_FirstStageVoltageFeedback(TOSLINKDuplexChannel* ch) noexcept;
};