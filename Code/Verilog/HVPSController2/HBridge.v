module HBridge(
    input wire clk,           // 50MHz system clock
    input wire can_drive,     // from BangBangController
	 input wire [1:0] drive_mode,//From CommandHandler
	 input wire [3:0] n_half_cycles_to_drive,
	 output reg done_finite_half_cycles,
	 input wire shut_down,
    output wire U23_HIN,
    output wire U23_LIN,
    output wire U23_SD,
    output wire U24_HIN,
    output wire U24_LIN,
    output wire U24_SD
);

// 50MHz / 16kHz = 3125 counts per full cycle
// Quarter cycle = 3125 / 4 = 781 counts
// Dead time - small gap between complementary switches
// to prevent shoot through

//localparam FULL_CYCLE    = 3125;
localparam QUARTER_CYCLE = 781;
localparam DEAD_TIME     = 5;    // 5 clocks = 100ns

reg [11:0] counter = 0;
reg [3:0]  quarter = 0;        // 0,1,2,3
reg drive_ended_for_quarter = 0;
reg [3:0] n_half_cycles_driven = 0;
reg hLeft = 0;
reg lLeft = 0;
reg hRight = 0;
reg lRight = 0;
assign U23_HIN = hLeft  & ~lLeft;
assign U23_LIN = lLeft  & ~hLeft;
assign U24_HIN = hRight & ~lRight;
assign U24_LIN = lRight & ~hRight;
assign U23_SD = shut_down;
assign U24_SD = shut_down;
always @(posedge clk) begin
    hLeft  <= 0;
    lLeft  <= 0;
    hRight <= 0;
    lRight <= 0;
    if (counter >= QUARTER_CYCLE - 1) begin
			counter <= 0;
			case(drive_mode)
				DRIVE_MODE_NO_DRIVE: begin
					n_half_cycles_driven <= 0;
					done_finite_half_cycles <= 0;
					drive_ended_for_quarter <= 1;
				end
				DRIVE_MODE_DRIVE:begin				
					n_half_cycles_driven <= 0;
					drive_ended_for_quarter <= ~can_drive;
					done_finite_half_cycles <= 0;
				end
				DRIVE_MODE_DRIVE_FINITE_HALF_CYCLES:begin				
					if(n_half_cycles_driven< n_half_cycles_to_drive) begin
						n_half_cycles_driven <= n_half_cycles_driven + 4'd1;
						drive_ended_for_quarter <= ~can_drive;
					end
					else begin
						done_finite_half_cycles <= 1;
						drive_ended_for_quarter<= 1;
					end
				end
				default:begin
					n_half_cycles_driven <= 0;
					done_finite_half_cycles <= 0;
					drive_ended_for_quarter <= 1;
				end
			endcase
			quarter <= (quarter >= 3) ? 4'd0 : quarter + 4'd1;
	 end else begin
        counter <= counter + 5'd1;
		  if(!can_drive)begin
				drive_ended_for_quarter <= 1;
		  end
    end
    case (quarter)
        0: begin
					// On quarter cycle - drive if permitted
				if (can_drive) begin
					if ( 
						(!drive_ended_for_quarter)
						&&(counter > DEAD_TIME) 
						&& (counter < QUARTER_CYCLE - DEAD_TIME)
					) begin
						  hLeft <= 1;
						  lRight <= 1;
					end
				end
			end
			1: begin
            // Off quarter cycle - nothing drives
				//drive_ended_for_quarter <= 0;
			end
			2: begin
            // On quarter cycle opposite polarity - drive if permitted
				if (can_drive) begin
					if (
						(!drive_ended_for_quarter)
						&&(counter > DEAD_TIME) 
						&&(counter < QUARTER_CYCLE - DEAD_TIME)) begin
					  hRight <= 1;
					  lLeft <= 1;
					end
				end
			end
        3: begin
            // Off quarter cycle - nothing drives
				//drive_ended_for_quarter <= 0;
        end
    endcase

end

endmodule