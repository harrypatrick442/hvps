#ifndef OUTPUTS_HPP
#define OUTPUTS_HPP
class Outputs{
	
	private:
        static const char* TAG;
		static bool s_initialized;
		static bool s_safe;
		static void configureOutputPin(int pin, bool onElseOff = false);
		static void _setMOSFETOnOff(bool onElseOff);
	public: 
		static void initialize();
		static void toSafe();
		static void setMOSFETOnOff(bool onElseOff);
};
#endif