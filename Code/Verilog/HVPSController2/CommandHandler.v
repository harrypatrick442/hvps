module CommandHandler(
	input wire clk_50Mhz,
	input wire [7:0] command,
	output reg [7:0] state,
	output reg done_command,
	output reg [1:0] drive_mode,
	output reg [3:0] n_half_cycles_to_drive,
	output reg shut_down_h_bridge,
	input wire done_finite_half_cycles
);
initial begin

end
reg [7:0] last_command = 8'b00000000;
always @(posedge clk_50Mhz) begin
	if(last_command!=command)begin
		last_command <= command;
		case(command)
			FPGA_COMMAND_NONE:begin
				drive_mode<=DRIVE_MODE_NO_DRIVE;
				shut_down_h_bridge <= 1;
			end
			FPGA_COMMAND_IDLE:begin
				drive_mode<=DRIVE_MODE_NO_DRIVE;
				shut_down_h_bridge <= 1;
			end
			FPGA_COMMAND_DRIVE:begin
				drive_mode<=DRIVE_MODE_DRIVE;
				shut_down_h_bridge <= 0;
			end
			FPGA_COMMAND_SAMPLE_ONE_CYCLE:begin
				if(command==last_command)begin
					if(done_finite_half_cycles)begin
						drive_mode<=DRIVE_MODE_NO_DRIVE;
						shut_down_h_bridge <= 1;
						done_command <= 1;
					end
				end
				else begin
					n_half_cycles_to_drive<=4'b0010;
					drive_mode<=DRIVE_MODE_DRIVE_FINITE_HALF_CYCLES;
					shut_down_h_bridge <= 0;
				end
			end
			default:begin
				drive_mode<=DRIVE_MODE_NO_DRIVE;
				shut_down_h_bridge <= 1;
			end
		endcase
	end
end
endmodule