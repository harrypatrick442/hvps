#ifndef PinDefinitions_hpp
#define PinDefinitions_hpp
class PinDefinitions{
	public:
        static int OUTPUT_VOLTAGE_FEEDBACK_RX;
        static int OUTPUT_VOLTAGE_FEEDBACK_TX;
        static int OUTPUT_VOLTAGE_FEEDBACK_THRESHOLD_REACHED;
        static int FIRST_STAGE_VOLTAGE_FEEDBACK_RX;
        static int FIRST_STAGE_VOLTAGE_FEEDBACK_TX;
        static int FIRST_STAGE_VOLTAGE_FEEDBACK_THRESHOLD_REACHED;
		static int OUTPUT_CURRENT_FEEDBACK_TX;
		static int OUTPUT_CURRENT_FEEDBACK_RX;
		static int OUTPUT_CURRENT_FEEDBACK_THRESHOLD_REACHED;
		static int PERIPHERALS_TX;
		static int PERIPHERALS_RX;
		static int MOSFET_DRIVE;
		static int PRIMARY_CURRENT_FEEDBACK;
		static int POWER_SUPPLY_VOLTAGE_FEEDBACK;
		static int SNUBBER_VOLTAGE_FEEDBACK;
		static int REFERENCE_VOLTAGE;
};
#endif