module AD7822(
    input wire clk_50MHz,
    input wire DB0,
    input wire DB1,
    input wire DB2,
    input wire DB3,
    input wire DB4,
    input wire DB5,
    input wire DB6,
    input wire DB7,
    output reg INV_CONVST,
    input wire INV_EOC,
    output reg INV_RD,
    output reg [7:0] data,
    output reg [7:0] data2
);

// At 50MHz, 1 clock = 20ns
// Conversion time max = 420ns = 21 clocks
// RD pulse min = 30ns = 2 clocks
// Data access time after RD low = 20ns = 1 clock

localparam IDLE        = 2'd0;
localparam CONVERTING  = 2'd1;
localparam READING     = 2'd2;
localparam RD_HOLD     = 2'd3;

reg [1:0] state;
reg [4:0] counter;

initial begin
    INV_CONVST = 0;
    INV_RD     = 1;
    data       = 8'b00001111;
    data2       = 8'b00001111;
end

always @(posedge clk_50MHz) begin
    case (state)
        IDLE: begin
            INV_CONVST <= 0; // falling edge starts conversion
            INV_RD     <= 1;
            counter    <= 0;
            state      <= CONVERTING;
        end

        CONVERTING: begin
			 INV_CONVST <= 1;
			 if (!INV_EOC) begin
				  INV_RD  <= 0;
				  counter <= 0;
				  state   <= READING;
			 end else if (counter >= 5'd30) begin // 30 clocks = 600ns timeout
				  // EOC never came, retry
				  counter <= 0;
				  state   <= IDLE; // go back and try again
			 end else begin
				  counter <= counter + 5'd1;
			 end
		end

        READING: begin
            if (counter >= 1) begin // wait 2 clocks = 40ns > 20ns access time
			//at moment this is 60ns can do counter>=1 for 40ns probably.
                data <= {DB7,DB6,DB5,DB4,DB3,DB2,DB1,DB0};//{DB7,DB6,DB5,DB4,DB3,DB2,DB1,DB0};
                data2 <= {DB7,DB6,DB5,DB4,DB3,DB2,DB1,DB0};//{DB7,DB6,DB5,DB4,DB3,DB2,DB1,DB0};
                INV_RD <= 1;
                counter <= 0;
                state   <= RD_HOLD;
            end else begin
                counter <= counter + 5'd1;
            end
        end

        RD_HOLD: begin
            if (counter >= 2) begin // t3 = 30ns min after RD rising before next CONVST
			// t3 requires 30ns min (1.5 clocks) between RD rising and next CONVST falling
			// actual gap including RD_HOLD + IDLE entry = ~4 clocks = 80ns, comfortably meets spec
			//can probably change to counter>=0 or remove counter for 40ns.
                state   <= IDLE;
                counter <= 0;
            end else begin
                counter <= counter + 5'd1;;
            end
        end
    endcase
end

endmodule