module CaptureBuffer(
	 input wire clk_50Mhz,
	 input wire capture,
	 input wire [7:0] source,
	 input wire clk_source,
	 input wire [15:0] the_time,
	 input wire clk_out,
	 input wire[15:0] out_index,
	 output reg [7:0] n_values,
	 output reg [7:0] value_out,
	 output reg [15:0] time_out
);
reg [15:0] value_index;
reg [2039:0] values_buffer;//255 values
reg [4079:0] times_buffer;//255 two byte times
integer i;
integer j;
initial begin
    value_index = 8'b00000000;
    n_values= 8'b00000000;
end

always @(posedge clk_50Mhz) begin
	if(!capture)begin
		n_values <= value_index+1;
		value_index <= 8'b00000000;
	end
end
always @(posedge clk_source) begin
	if(value_index < 255) begin
		for (i = 0; i < 8; i = i + 1) begin
			values_buffer[(value_index*8) + i] = source[i];
		end
		for (i = 0; i < 16; i = i + 1) begin
			times_buffer[(value_index*16) + i] = the_time[i];
		end
		value_index <= value_index + 1;
	end
end
always @(posedge clk_out) begin
	if(out_index<n_values) begin
		for (j = 0; j < 8; j = j + 1) begin
			value_out[j] = values_buffer[(out_index*8) + j];
		end
		for (j = 0; j < 16; j = j + 1) begin
			time_out[j] = times_buffer[(out_index*16) + j];
		end
	end
end
endmodule