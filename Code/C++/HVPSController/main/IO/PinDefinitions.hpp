#ifndef PinDefinitions_hpp
#define PinDefinitions_hpp
class PinDefinitions{
	public:
        static const int OUTPUT_VOLTAGE_FEEDBACK_RX;
        static const int OUTPUT_VOLTAGE_FEEDBACK_TX;
        static const int OUTPUT_VOLTAGE_FEEDBACK_THRESHOLD_REACHED;
        static const int FIRST_STAGE_VOLTAGE_FEEDBACK_RX;
        static const int FIRST_STAGE_VOLTAGE_FEEDBACK_TX;
        static const int FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED;
		static const int OUTPUT_CURRENT_FEEDBACK_TX;
		static const int OUTPUT_CURRENT_FEEDBACK_RX;
		static const int OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED;
		static const int OTHER_PERIPHERALS_TX;
		static const int OTHER_PERIPHERALS_RX;
		static const int MOSFET_DRIVE;
		static const int PRIMARY_CURRENT_FEEDBACK;
		static const int POWER_SUPPLY_VOLTAGE_FEEDBACK;
		static const int SNUBBER_VOLTAGE_FEEDBACK;
		static const int REFERENCE_VOLTAGE;
		static const int SOFT_START_RESISTOR_BYPASS;
};
#endif