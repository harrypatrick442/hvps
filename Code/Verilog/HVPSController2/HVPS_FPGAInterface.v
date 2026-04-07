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
    output wire  drive,
    output wire  drive2,
    output wire [7:0] desired_max_first_stage_voltage,
    output wire [7:0] desired_output_voltage,
    output wire [7:0] desired_max_peak_primary_current,
    output wire [7:0] command,
    input wire [7:0] actual_first_stage_voltage,
    input wire [7:0] actual_output_voltage,
    input wire [7:0] actual_peak_primary_current,
    input wire [7:0] actual_first_stage_voltage2,
    input wire [7:0] actual_output_voltage2,
    input wire [7:0] actual_peak_primary_current2,
    input wire  error,
    input wire [7:0] echo_desired_max_first_stage_voltage,
    input wire [7:0] echo_desired_desired_output_voltage,
    input wire [7:0] echo_desired_max_peak_primary_current,
    input wire [959:0] collected_sample
);

    // Input buffer (staged, not yet live)
    reg [33:0] input_staged;
    // Live input buffer
    reg [33:0] input_live;
    // Full output shift buffer (inputs + outputs)
    reg [1066:0] output_buffer;

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
    assign drive = input_live[0];
    assign drive2 = input_live[1];
    assign desired_max_first_stage_voltage = input_live[9:2];
    assign desired_output_voltage = input_live[17:10];
    assign desired_max_peak_primary_current = input_live[25:18];
    assign command = input_live[33:26];
    assign out_value = output_buffer[1066];

    // Debouncer instantiations
    debouncer #(
        .DEBOUNCE_LIMIT(2500)
    ) debouncer_in_shift (
        .clk(clk),
        .bouncy_in(in_shift),
        .debounced_out(in_shift_debounced)
    );
    debouncer #(
        .DEBOUNCE_LIMIT(2500)
    ) debouncer_go_live (
        .clk(clk),
        .bouncy_in(go_live),
        .debounced_out(go_live_debounced)
    );
    debouncer #(
        .DEBOUNCE_LIMIT(2500)
    ) debouncer_to_output (
        .clk(clk),
        .bouncy_in(to_output),
        .debounced_out(to_output_debounced)
    );
    debouncer #(
        .DEBOUNCE_LIMIT(2500)
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
            input_staged <= {input_staged[32:0], in_value};
        end

        // Go live - rising edge of go_live
        if (go_live_debounced && !go_live_prev) begin
            input_live <= input_staged;
        end

        // Output buffer - rising edge of to_output or out_shift
        if (to_output_debounced && !to_output_prev) begin
            output_buffer <= {collected_sample, echo_desired_max_peak_primary_current, echo_desired_desired_output_voltage, echo_desired_max_first_stage_voltage, error, actual_peak_primary_current2, actual_output_voltage2, actual_first_stage_voltage2, actual_peak_primary_current, actual_output_voltage, actual_first_stage_voltage, input_staged};
        end else if (out_shift_debounced && !out_shift_prev) begin
            output_buffer <= {output_buffer[1065:0], 1'b0};
        end

    end

endmodule
