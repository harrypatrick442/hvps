//FPGA EP4CE6E22C8N
//flash is winbond  25Q16JVN10 2430
//Go to File->Convert Programming Files
//EPCS16 is what we select as the flash.
//convert to .jic file. Thats what we flash.
//delete the second chip that shows up sometimes with the 22 on the end in the programmer

module hvps_controller(
    input  wire clk,

    input wire ESP_EMERGENCY_STOP,
    input wire ESP_GO_LIVE, 
    input wire ESP_IN_SHIFT,
    input wire ESP_IN_VALUE,
    input wire ESP_OUT_SHIFT,
    output wire ESP_OUT_VALUE,
    input wire ESP_TO_OUTPUT,
	 input wire OR4,
	 input wire OR5,
	 input wire OR6,
	 input wire OT4,
	 input wire OT5,
	 input wire U5_DB0,
	 input wire U5_DB1,
	 input wire U5_DB2,
	 input wire U5_DB3,
	 input wire U5_DB4,
	 input wire U5_DB5,
	 input wire U5_DB6,
	 input wire U5_DB7,
	 input wire U5_INV_CONVST,
	 input wire U5_INV_EOC,
	 input wire U5_INV_RD,
    input wire U12_DB0,
    input wire U12_DB1,
    input wire U12_DB2,
    input wire U12_DB3,
    input wire U12_DB4,
    input wire U12_DB5,
    input wire U12_DB6,
    input wire U12_DB7,
    input wire U12_INV_CONVST,
	 input wire U12_INV_EOC,
	 input wire U12_INV_RD,
    input wire U20_DB0,
    input wire U20_DB1,
    input wire U20_DB2,
    input wire U20_DB3,
    input wire U20_DB4,
    input wire U20_DB5,
    input wire U20_DB6,
    input wire U20_DB7,
    input wire U20_INV_CONVST,
	 input wire U20_INV_EOC,
	 input wire U20_INV_RD,
	 input wire U23_HIN,
	 input wire U23_LIN,
	 input wire U23_SD,
	 input wire U24_HIN,
	 input wire U24_LIN,
	 input wire U24_SD
);

    // Wires to connect to the interface
    wire drive;
    wire drive2;
    wire [7:0] desired_max_first_stage_voltage;
    wire [7:0] desired_output_voltage;
    wire [7:0] desired_max_peak_primary_current;
    
    // These would come from your actual core logic
    reg [7:0] actual_first_stage_voltage =  8'b01010101;
    reg [7:0] actual_output_voltage = 8'b00000111;
    reg [7:0] actual_peak_primary_current = 8'b00001111;
    reg fpga_in_error = 1;
    //assign ESP_OUT_VALUE = 1'b1;
	 
    // Instantiate the module
    HVPS_FPGAInterface fpga_interface (
		  .clk(clk),
        .in_shift(ESP_IN_SHIFT),
        .in_value(ESP_IN_VALUE),
        .out_value(ESP_OUT_VALUE),
        .out_shift(ESP_OUT_SHIFT),
        .to_output(ESP_TO_OUTPUT),
        .go_live(ESP_GO_LIVE),
        .drive(drive),
        .drive2(drive2),
        .desired_max_first_stage_voltage(desired_max_first_stage_voltage),
        .desired_output_voltage(desired_output_voltage),
        .desired_max_peak_primary_current(desired_max_peak_primary_current),
        .actual_first_stage_voltage(actual_first_stage_voltage),
        .actual_output_voltage(actual_output_voltage),
        .actual_peak_primary_current(actual_peak_primary_current),
        .error(fpga_in_error)
    );

endmodule