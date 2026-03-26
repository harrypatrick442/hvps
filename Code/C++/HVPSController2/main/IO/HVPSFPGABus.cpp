#include "HVPSFPGABus.hpp"
#include "Macros/GetFileName.hpp"
#include "IO/PinDefinitions.hpp"
const char* HVPSFPGABus::getTag() {return GET_FILE_NAME;}
HVPSFPGABus::HVPSFPGABus(
	)noexcept:FPGABus(
		PinDefinitions::FPGA_IN_SHIFT, 
		PinDefinitions::FPGA_IN_VALUE, 
		PinDefinitions::FPGA_TO_OUTPUT, 
		PinDefinitions::FPGA_OUT_SHIFT, 
		PinDefinitions::FPGA_OUT_VALUE, 
		PinDefinitions::FPGA_GO_LIVE
	){
		
	}