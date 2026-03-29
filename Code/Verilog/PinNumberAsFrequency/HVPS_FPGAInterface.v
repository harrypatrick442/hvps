module HVPS_FPGAInterface (
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
    output wire [7:0] desired_max_first_stage_voltage,
    output wire [7:0] desired_output_voltage,
    output wire [7:0] desired_max_peak_primary_current,
    input wire [7:0] actual_first_stage_voltage,
    input wire [7:0] actual_output_voltage,
    input wire [7:0] actual_peak_primary_current,
    input wire  error
);

    // Input buffer (staged, not yet live)
    reg [25:0] input_staged;
    // Live input buffer
    reg [25:0] input_live;
    // Full output shift buffer (inputs + outputs)
    reg [50:0] output_buffer;
    // Shift counter
    integer shift_count;

    // Named signal assignments from live input buffer
    assign drive = input_live[0];
    assign drive2 = input_live[1];
    assign desired_max_first_stage_voltage = input_live[9:2];
    assign desired_output_voltage = input_live[17:10];
    assign desired_max_peak_primary_current = input_live[25:18];

    // Shift in - MSB first
    always @(posedge in_shift) begin
        input_staged <= {input_staged[24:0], in_value};
    end

    // Go live - commit staged to live
    always @(posedge go_live) begin
        input_live <= input_staged;
    end

    // Load output buffer when to_output pulses
    always @(posedge to_output) begin
        output_buffer <= {actual_first_stage_voltage, actual_output_voltage, actual_peak_primary_current, error, input_staged};
    end

    // Shift out - MSB first
    always @(posedge out_shift) begin
        out_value <= output_buffer[50];
        output_buffer <= {output_buffer[49:0], 1'b0};
    end

endmodule
