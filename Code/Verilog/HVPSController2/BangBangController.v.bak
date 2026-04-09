module BangBangController(
    input wire [7:0] primary_current_raw,
    input wire [7:0] first_stage_voltage_raw,
    input wire [7:0] output_voltage_raw,
	input wire drive,
	input wire drive2,
    input wire [7:0] desired_max_first_stage_voltage,
    input wire [7:0] desired_output_voltage,
    input wire [7:0] desired_max_peak_primary_current,
    output wire can_drive
);
	assign can_drive =
			/*drive signals from the microcontroller like a turn on signal*/ drive && drive2
			/*peak primary current*/&&(primary_current_raw < desired_max_peak_primary_current)
            /*first stage voltage */&&(first_stage_voltage_raw < desired_max_first_stage_voltage)
			/*output voltage */ 	&&(output_voltage_raw< desired_output_voltage);
endmodule