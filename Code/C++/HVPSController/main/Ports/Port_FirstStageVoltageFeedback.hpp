#pragma once
#include "Port_VoltageFeedbackBase.hpp"
#include "../Core/SingletonBase.hpp"
#include "../Core/Macros.hpp"
class Port_FirstStageVoltageFeedback final :
	public Port_VoltageFeedbackBase,
	public SingletonBase<Port_FirstStageVoltageFeedback>{
    public :
		static inline constexpr const char* TAG = "Port_FirstStageVoltageFeedback";
		DISALLOW_COPY_MOVE(LiveDataCache);
		const char* getTag() const override { return TAG; }
		static Port_FirstStageVoltageFeedback& initialize() noexcept;
    private:
		friend class SingletonBase<Port_FirstStageVoltageFeedback>; // lets SingletonBase call our ctor
		explicit Port_FirstStageVoltageFeedback(TOSLINKDuplexChannel* ch) noexcept;
};