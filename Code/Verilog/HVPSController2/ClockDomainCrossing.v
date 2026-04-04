module ClockDomainCrossingFastToSlow8(
    input wire clock_from_fast,
    input wire clock_to_slow,
    input reg [7:0] reg_from_fast,
    input reg [7:0] reg_to_slow
);

reg clock_to_slow_was_low;

always @(posedge clock_from_fast or negedge clock_to_slow) begin
    if (!clock_to_slow)
        clock_to_slow_was_low <= 1;
    else if (clock_to_slow && clock_to_slow_was_low) begin
        clock_to_slow_was_low <= 0;
        reg_to_slow <= reg_from_fast;
    end
end