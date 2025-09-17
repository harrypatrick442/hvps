#pragma once
#include "Port_VoltageFeedbackBase.hpp"
#include "../Core/SingletonBase.hpp"
#include "../Core/Macros.hpp"
class Port_OutputVoltageFeedback final :
	public Port_VoltageFeedbackBase,
	public SingletonBase<Port_OutputVoltageFeedback>{
    public :
		static inline constexpr const char* TAG = "Port_OutputVoltageFeedback";
		const char* getTag() const override { return TAG; }
		static Port_OutputVoltageFeedback& initialize() noexcept;
		DISALLOW_COPY_MOVE(Port_OutputVoltageFeedback);
    private:
		friend class SingletonBase<Port_OutputVoltageFeedback>; // lets SingletonBase call our ctor
		explicit Port_OutputVoltageFeedback(TOSLINKDuplexChannel* ch) noexcept;
};