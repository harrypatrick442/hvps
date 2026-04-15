module HVPS_FPGAInterface (
    // Shift register bus pins
    input wire clk,
    input wire in_shift,
    input wire in_value,
    output wire out_value,
    input wire out_shift,
    input wire to_output,
    input wire go_live,
    // Named outputs to core logic
    output wire [7:0] command,
    output wire [7:0] desired_output_voltage,
    input wire [1023:0] buffered_data,
    input wire [7:0] echo_desired_output_voltage,
    input wire [7:0] max_primary_current,
    input wire [7:0] max_output_voltage,
    input wire [7:0] max_first_stage_voltage,
    input wire [7:0] state,
    input wire [7:0] actual_peak_primary_current,
    input wire [7:0] actual_output_voltage,
    input wire [7:0] actual_first_stage_voltage,
    input wire  done_command
);

    // Input buffer (staged, not yet live)
    reg [15:0] input_staged;
    // Live input buffer
    reg [15:0] input_live;
    // Full output shift buffer (inputs + outputs)
    reg [1104:0] output_buffer;

    // Edge detection registers
    reg in_shift_prev;
    reg go_live_prev;
    reg to_output_prev;
    reg out_shift_prev;

    // Debounced signal wires
    wire in_shift_debounced;
    wire go_live_debounced;
    wire to_output_debounced;
    wire out_shift_debounced;

    // Named signal assignments from live input buffer
    assign command = input_live[7:0];
    assign desired_output_voltage = input_live[15:8];
    assign out_value = output_buffer[1104];

    // Debouncer instantiations
    debouncer #(
        .DEBOUNCE_LIMIT(3750)
    ) debouncer_in_shift (
        .clk(clk),
        .bouncy_in(in_shift),
        .debounced_out(in_shift_debounced)
    );
    debouncer #(
        .DEBOUNCE_LIMIT(3750)
    ) debouncer_go_live (
        .clk(clk),
        .bouncy_in(go_live),
        .debounced_out(go_live_debounced)
    );
    debouncer #(
        .DEBOUNCE_LIMIT(3750)
    ) debouncer_to_output (
        .clk(clk),
        .bouncy_in(to_output),
        .debounced_out(to_output_debounced)
    );
    debouncer #(
        .DEBOUNCE_LIMIT(3750)
    ) debouncer_out_shift (
        .clk(clk),
        .bouncy_in(out_shift),
        .debounced_out(out_shift_debounced)
    );

    // All logic synchronous to system clock
    always @(posedge clk) begin

        // Update edge detection registers
        in_shift_prev  <= in_shift_debounced;
        go_live_prev   <= go_live_debounced;
        to_output_prev <= to_output_debounced;
        out_shift_prev <= out_shift_debounced;

        // Shift in - rising edge of in_shift
        if (in_shift_debounced && !in_shift_prev) begin
            input_staged <= {input_staged[14:0], in_value};
        end

        // Go live - rising edge of go_live
        if (go_live_debounced && !go_live_prev) begin
            input_live <= input_staged;
        end

        // Output buffer - rising edge of to_output or out_shift
        if (to_output_debounced && !to_output_prev) begin
            output_buffer <= {done_command, actual_first_stage_voltage, actual_output_voltage, actual_peak_primary_current, state, max_first_stage_voltage, max_output_voltage, max_primary_current, echo_desired_output_voltage, buffered_data, input_staged};
        end else if (out_shift_debounced && !out_shift_prev) begin
            output_buffer <= {output_buffer[1103:0], 1'b0};
        end

    end

endmodule
