module debouncer #(
    parameter DEBOUNCE_LIMIT = 500_000 // 10ms for a 50MHz clock
)(
    input  clk,
    input  bouncy_in,
    output reg debounced_out
);

    // 1. Synchronize the asynchronous input to the clock domain
    reg sync_0, sync_1;
    always @(posedge clk) begin
        sync_0 <= bouncy_in;
        sync_1 <= sync_0;
    end

    // 2. Counter-based filtering logic
    reg [18:0] counter = 0; // 19 bits can hold 500,000
    
    always @(posedge clk) begin
        // If synchronized input differs from the current stable output
        if (sync_1 != debounced_out) begin
            if (counter < DEBOUNCE_LIMIT) begin
                counter <= counter + 1'b1;
            end else begin
                // Input has been stable for the limit; update output
                debounced_out <= sync_1;
                counter <= 0;
            end
        end else begin
            // Input matches output; reset counter
            counter <= 0;
        end
    end

endmodule