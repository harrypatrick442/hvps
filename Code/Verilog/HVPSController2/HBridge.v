module HBridge(
    input wire clk,           // 50MHz system clock
    input wire can_drive,     // from BangBangController
    output reg U23_HIN,
    output reg U23_LIN,
    output reg U23_SD,
    output reg U24_HIN,
    output reg U24_LIN,
    output reg U24_SD
);

// 50MHz / 16kHz = 3125 counts per full cycle
// Quarter cycle = 3125 / 4 = 781 counts
// Dead time - small gap between complementary switches
// to prevent shoot through

localparam FULL_CYCLE    = 3125;
localparam QUARTER_CYCLE = 781;
localparam DEAD_TIME     = 5;    // 5 clocks = 100ns

reg [11:0] counter;
reg [3:0]  quarter;        // 0,1,2,3
reg drive_ended_for_quarter = 0;
always @(posedge clk) begin

    if (counter >= QUARTER_CYCLE - 1) begin
        counter <= 0;
        quarter <= (quarter == 3) ? 0 : quarter + 1;
		drive_ended_for_quarter <= 0;
    end else begin
        counter <= counter + 1;
    end

    // Default - everything off
    U23_HIN <= 0;
    U23_LIN <= 0;
    U24_HIN <= 0;
    U24_LIN <= 0;
    U23_SD  <= 1;   // SD high = enabled
    U24_SD  <= 1;

    case (quarter)
        0: begin
            // On quarter cycle - drive if permitted
            if (can_drive) begin
                if ( 
					(!drive_ended_for_quarter)
					&&(counter > DEAD_TIME) 
					&& (counter < QUARTER_CYCLE - DEAD_TIME)
				) begin
                    U23_HIN <= 1;
                    U24_LIN <= 1;
                end
            end else begin
				drive_ended_for_quarter <= 1;
			end
        end
        1: begin
            // Off quarter cycle - nothing drives
			drive_ended_for_quarter <= 0;
        end
        2: begin
            // On quarter cycle opposite polarity - drive if permitted
            if (can_drive) begin
                if (
					(!drive_ended_for_quarter)
					&&(counter > DEAD_TIME) 
					&&(counter < QUARTER_CYCLE - DEAD_TIME)) begin
                    U24_HIN <= 1;
                    U23_LIN <= 1;
                end
            end else begin
				drive_ended_for_quarter <= 1;
			end
        end
        3: begin
            // Off quarter cycle - nothing drives
			drive_ended_for_quarter <= 0;
        end
    endcase

end

endmodule