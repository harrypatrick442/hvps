module CommandHandler(
	input wire clk_50Mhz,
	input wire [FPGA_COMMAND_LAST_INDEX:0] command,
	output reg [FPGA_STATE_LAST_INDEX:0] state,
	output reg [1:0] drive_mode,
	output reg [3:0] n_quarter_cycles_to_drive,
	output reg shut_down_h_bridge,
	input wire done_finite_quarter_cycles
);
initial begin

end
reg [7:0] last_command = 8'b00000000;
always @(posedge clk_50Mhz) begin
	last_command <= command;
	case(command)
		FPGA_COMMAND_NONE:begin
			drive_mode<=DRIVE_MODE_NO_DRIVE;
			shut_down_h_bridge <= 1;
			state <= FPGA_STATE_NONE;
		end
		FPGA_COMMAND_DRIVE:begin
			drive_mode<=DRIVE_MODE_DRIVE;
			shut_down_h_bridge <= 0;
			state <= FPGA_STATE_DRIVING;
		end
		FPGA_COMMAND_SAMPLE_HALF_CYCLE:begin
			if(command==last_command)begin
				if(done_finite_quarter_cycles)begin
					drive_mode<=DRIVE_MODE_NO_DRIVE;
					shut_down_h_bridge <= 1;
					state <= FPGA_STATE_SAMPLED_HALF_CYCLE;
				end
			end
			else begin
				n_quarter_cycles_to_drive<=4'b0010;
				drive_mode<=DRIVE_MODE_DRIVE_FINITE_QUARTER_CYCLES;
				shut_down_h_bridge <= 0;
				state <= FPGA_STATE_SAMPLING_HALF_CYCLE;
			end
		end
		FPGA_COMMAND_SAMPLE_FULL_CYCLE:begin
			if(command==last_command)begin
				if(done_finite_quarter_cycles)begin
					drive_mode<=DRIVE_MODE_NO_DRIVE;
					shut_down_h_bridge <= 1;
					state <= FPGA_STATE_SAMPLED_FULL_CYCLE;
				end
			end
			else begin
				n_quarter_cycles_to_drive<=4'b0100;
				drive_mode<=DRIVE_MODE_DRIVE_FINITE_QUARTER_CYCLES;
				shut_down_h_bridge <= 0;
				state <= FPGA_STATE_SAMPLING_FULL_CYCLE;
			end
		end
		FPGA_COMMAND_READ_NEXT_DATA_BYTES:begin
			drive_mode<=DRIVE_MODE_NO_DRIVE;
			shut_down_h_bridge <= 1;
			state <= FPGA_STATE_NEXT_DATA_BYTES;
			if(command!=last_command)begin
				//Shift next bytes to be bussed out.
			end
		end
		default:begin
			drive_mode<=DRIVE_MODE_NO_DRIVE;
			shut_down_h_bridge <= 1;
		end
	endcase
end
endmodule