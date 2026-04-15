module BangBangController(
    input wire [7:0] primary_current_raw,
    input wire [7:0] first_stage_voltage_raw,
    input wire [7:0] output_voltage_raw,
    input wire [7:0] desired_output_voltage,
    output wire can_drive
);
	assign can_drive =
			/*peak primary current*/(primary_current_raw < MAX_PRIMARY_CURRENT)
         /*first stage voltage */&&(first_stage_voltage_raw < MAX_FIRST_STAGE_VOLTAGE)
			/*output voltage */ 		&&(output_voltage_raw < desired_output_voltage)
											&&(output_voltage_raw < MAX_OUTPUT_VOLTAGE);
endmodule