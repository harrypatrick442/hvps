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
	 output wire OT5,
	 input wire U5_DB0,
	 input wire U5_DB1,
	 input wire U5_DB2,
	 input wire U5_DB3,
	 input wire U5_DB4,
	 input wire U5_DB5,
	 input wire U5_DB6,
	 input wire U5_DB7,
	 output wire U5_INV_CONVST,
	 input wire U5_INV_EOC,
	 output wire U5_INV_RD,
    input wire U12_DB0,
    input wire U12_DB1,
    input wire U12_DB2,
    input wire U12_DB3,
    input wire U12_DB4,
    input wire U12_DB5,
    input wire U12_DB6,
    input wire U12_DB7,
    output wire U12_INV_CONVST,
	 input wire U12_INV_EOC,
	 output wire U12_INV_RD,
    input wire U20_DB0,
    input wire U20_DB1,
    input wire U20_DB2,
    input wire U20_DB3,
    input wire U20_DB4,
    input wire U20_DB5,
    input wire U20_DB6,
    input wire U20_DB7,
    output wire U20_INV_CONVST,
	 input wire U20_INV_EOC,
	 output wire U20_INV_RD,
	 output wire U23_HIN,
	 output wire U23_LIN,
	 output wire U23_SD,
	 output wire U24_HIN,
	 output wire U24_LIN,
	 output wire U24_SD
);

    // Wires to connect to the interface
    wire drive;
    wire drive2;
    wire [7:0] desired_output_voltage;
    wire [7:0] command;
    
    // These would come from your actual core logic
    wire [7:0] primary_current_raw;
    wire [7:0] first_stage_voltage_raw;
    wire [7:0] output_voltage_raw;
	 reg [1023:0] buffered_data;
    reg fpga_in_error = 1;
	 
	 wire can_drive;//The sacred signal from the bangbang controller that goes into the H-Bridge.
	 assign OT5 = drive & drive2;
	 
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
        .desired_output_voltage(desired_output_voltage),
        .command(command),
        .actual_first_stage_voltage(first_stage_voltage_raw),
        .actual_output_voltage(output_voltage_raw),
        .actual_peak_primary_current(primary_current_raw),
        .error(fpga_in_error),
        .max_first_stage_voltage(MAX_FIRST_STAGE_VOLTAGE),
        .max_output_voltage(MAX_OUTPUT_VOLTAGE),
        .max_primary_current(MAX_PRIMARY_CURRENT),
        .echo_desired_output_voltage(desired_output_voltage),
        .buffered_data(buffered_data)
    );
	 
	 
	 AD7822 primary_current_feedback_adc(
		 .clk_50MHz(clk),
		 .DB0(U12_DB0),
		 .DB1(U12_DB1),
		 .DB2(U12_DB2),
		 .DB3(U12_DB3),
		 .DB4(U12_DB4),
		 .DB5(U12_DB5),
		 .DB6(U12_DB6),
		 .DB7(U12_DB7),
		 .INV_CONVST(U12_INV_CONVST),
		 .INV_EOC(U12_INV_EOC),
		 .INV_RD(U12_INV_RD),
		 .data(primary_current_raw)
	  );
	 AD7822 output_voltage_feedback_adc(
		 .clk_50MHz(clk),
		 .DB0(U5_DB0),
		 .DB1(U5_DB1),
		 .DB2(U5_DB2),
		 .DB3(U5_DB3),
		 .DB4(U5_DB4),
		 .DB5(U5_DB5),
		 .DB6(U5_DB6),
		 .DB7(U5_DB7),
		 .INV_CONVST(U5_INV_CONVST),
		 .INV_EOC(U5_INV_EOC),
		 .INV_RD(U5_INV_RD),
		 .data(output_voltage_raw)
	  );
	 AD7822 first_stage_voltage_feedback_adc(
		 .clk_50MHz(clk),
		 .DB0(U20_DB0),
		 .DB1(U20_DB1),
		 .DB2(U20_DB2),
		 .DB3(U20_DB3),
		 .DB4(U20_DB4),
		 .DB5(U20_DB5),
		 .DB6(U20_DB6),
		 .DB7(U20_DB7),
		 .INV_CONVST(U20_INV_CONVST),
		 .INV_EOC(U20_INV_EOC),
		 .INV_RD(U20_INV_RD),
		 .data(first_stage_voltage_raw)
	  );
	  BangBangController bang_bang_controller(
		 .primary_current_raw(primary_current_raw),
		 .first_stage_voltage_raw(first_stage_voltage_raw),
		 .output_voltage_raw(output_voltage_raw),
		 .drive(drive),
		 .drive2(drive2),
		 .desired_output_voltage(desired_output_voltage),
		 .can_drive(can_drive)
	  );
	  HBridge hBridge(
		 .clk(clk),           // 50MHz system clock
		 .can_drive(drive),    // from BangBangController
		 .U23_HIN(U23_HIN),
		 .U23_LIN(U23_LIN),
		 .U23_SD(U23_SD),
		 .U24_HIN(U24_HIN),
		 .U24_LIN(U24_LIN),
		 .U24_SD(U24_SD)
	  );
	  
endmodule