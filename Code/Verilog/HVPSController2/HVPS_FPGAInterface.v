module HVPS_FPGAInterface (
    input wire clk,
    // Shift register bus pins
    input wire in_shift,
    input wire in_value,
    output reg out_value,
    input wire out_shift,
    input wire to_output,
    input wire go_live,
    // Named outputs to core logic
    output wire  drive,
    output wire  drive2,
    output wire [7:0] max_first_stage_voltage_allowed,
    output wire [7:0] desired_output_voltage,
    input wire [7:0] actual_first_stage_voltage,
    input wire [7:0] actual_output_voltage,
    input wire [7:0] actual_primary_current
);

    // Input buffer (staged, not yet live)
    reg [17:0] input_staged;
    // Live input buffer
    reg [17:0] input_live;
    // Full output shift buffer (inputs + outputs)
    reg [41:0] output_buffer;
    // Shift counter
    integer shift_count;

    // Named signal assignments from live input buffer
    assign drive = input_live[0];
    assign drive2 = input_live[1];
    assign max_first_stage_voltage_allowed = input_live[9:2];
    assign desired_output_voltage = input_live[17:10];

    // Shift in - MSB first
    always @(posedge in_shift) begin
        input_staged <= {input_staged[16:0], in_value};
    end

    // Go live - commit staged to live
    always @(posedge go_live) begin
        input_live <= input_staged;
    end

    // Load output buffer when to_output pulses
    always @(posedge to_output) begin
        output_buffer <= {actual_first_stage_voltage, actual_output_voltage, actual_primary_current, input_staged};
    end

    // Shift out - MSB first
    always @(posedge out_shift) begin
        out_value <= output_buffer[41];
        output_buffer <= {output_buffer[40:0], 1'b0};
    end

endmodule
