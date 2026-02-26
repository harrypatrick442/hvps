module hvps_controller(
    input  wire clk,

    output reg ESP_EMERGENCY_SHUTDOWN,
    output reg ESP_GO_LIVE, 
    output reg ESP_IN_SHIFT,
    output reg ESP_IN_VALUE,
    output reg ESP_OUT_SHIFT,
    output reg ESP_OUT_VALUE,
    output reg ESP_TO_OUTPUT,
	 output reg OR4,
	 output reg OR5,
	 output reg OR6,
	 output reg OT4,
	 output reg OT5,
	 output reg U5_DB0,
	 output reg U5_DB1,
	 output reg U5_DB2,
	 output reg U5_DB3,
	 output reg U5_DB4,
	 output reg U5_DB5,
	 output reg U5_DB6,
	 output reg U5_DB7,
	 output reg U5_INV_CONVST,
	 output reg U5_INV_EOC,
	 output reg U5_INV_RD,
    output reg U12_DB0,
    output reg U12_DB1,
    output reg U12_DB2,
    output reg U12_DB3,
    output reg U12_DB4,
    output reg U12_DB5,
    output reg U12_DB6,
    output reg U12_DB7,
    output reg U12_INV_CONVST,
	 output reg U12_INV_EOC,
	 output reg U12_INV_RD,
    output reg U20_DB0,
    output reg U20_DB1,
    output reg U20_DB2,
    output reg U20_DB3,
    output reg U20_DB4,
    output reg U20_DB5,
    output reg U20_DB6,
    output reg U20_DB7,
    output reg U20_INV_CONVST,
	 output reg U20_INV_EOC,
	 output reg U20_INV_RD,
	 output reg U23_HIN,
	 output reg U23_LIN,
	 output reg U23_SD,
	 output reg U24_HIN,
	 output reg U24_LIN,
	 output reg U24_SD
);

    // ------------------------------------------------------------
    // Internal state
    // ------------------------------------------------------------
	 localparam NUM_PINS = 51;
    reg [31:0] counters [0:NUM_PINS-1];
    reg [NUM_PINS-1:0] outputs;

    // ------------------------------------------------------------
    // Safe, wide frequency table (Hz)
    // ------------------------------------------------------------
 // ------------------------------------------------------------
// Frequency table = physical pin numbers
// ------------------------------------------------------------
localparam [16*NUM_PINS-1:0] PIN_FREQ = {
    16'd71,   // ESP_EMERGENCY_SHUTDOWN/
    16'd54,   // ESP_GO_LIVE/
    16'd50,   // ESP_IN_SHIFT/
    16'd52,   // ESP_IN_VALUE/
    16'd60,   // ESP_OUT_SHIFT/
    16'd69,   // ESP_OUT_VALUE/
    16'd58,   // ESP_TO_OUTPUT/
    16'd43,   // OR4/
    16'd39,   // OR5/
    16'd30,   // OR6/
    16'd28,    // OT4/
    16'd38,    // OT5/
    16'd74,   // U5_DB0/
    16'd84,   // U5_DB1/
    16'd85,   // U5_DB2/
    16'd72,   // U5_DB3/
    16'd70,   // U5_DB4/
    16'd68,   // U5_DB5/
    16'd64,   // U5_DB6/
    16'd59,   // U5_DB7/
    16'd83,   // U5_INV_CONVST/
    16'd73,   // U5_INV_EOC/
    16'd75,   // U5_INV_RD/
    16'd120,  // U12_DB0/
    16'd124,  // U12_DB1/
    16'd126,  // U12_DB2/
    16'd1000,    // U12_DB3/
    16'd143,  // U12_DB4/
    16'd141,  // U12_DB5/
    16'd135,  // U12_DB6/
    16'd128,  // U12_DB7/
    16'd113,  // U12_INV_CONVST/
    16'd106,  // U12_INV_EOC/
    16'd111,  // U12_INV_RD/
    16'd49,   // U20_DB0/
    16'd44,   // U20_DB1/
    16'd42,   // U20_DB2/
    16'd142,  // U20_DB3/
    16'd129,  // U20_DB4/
    16'd127,  // U20_DB5/
    16'd125,  // U20_DB6/
    16'd121,  // U20_DB7/
    16'd51,   // U20_INV_CONVST/
    16'd55,   // U20_INV_EOC/
    16'd53,   // U20_INV_RD/
    16'd144,  // U23_HIN/
    16'd3000,    // U23_LIN/
    16'd2000,    // U23_SD/
    16'd112,  // U24_HIN/
    16'd104,  // U24_LIN/
    16'd110,  // U24_SD/
};



    // ------------------------------------------------------------
    // Power-up initialisation (FPGA-safe)
    // ------------------------------------------------------------
    integer k;
    initial begin
        for (k = 0; k < 51; k = k + 1) begin
            counters[k] = 0;
            outputs[k]  = 0;
        end
    end

    // ------------------------------------------------------------
    // Clocked toggle engine
    // ------------------------------------------------------------
    integer j;
    always @(posedge clk) begin
        for (j = 0; j < 51; j = j + 1) begin
            if (counters[j] >= (50_000_000 / (2 * PIN_FREQ[16*(NUM_PINS-1-j) +: 16]))) begin
                counters[j] <= 0;
                outputs[j]  <= ~outputs[j];
            end else begin
                counters[j] <= counters[j] + 1;
            end
        end
    end

    // ------------------------------------------------------------
    // Explicit output mapping (THIS IS THE IMPORTANT PART)
    // ------------------------------------------------------------
   always @(*) begin
    ESP_EMERGENCY_SHUTDOWN = outputs[0];
    ESP_GO_LIVE            = outputs[1];
    ESP_IN_SHIFT           = outputs[2];
    ESP_IN_VALUE           = outputs[3];
    ESP_OUT_SHIFT          = outputs[4];
    ESP_OUT_VALUE          = outputs[5];
    ESP_TO_OUTPUT          = outputs[6];

    OR4 = outputs[7];
    OR5 = outputs[8];
    OR6 = outputs[9];
    OT4 = outputs[10];
    OT5 = outputs[11];

    U5_DB0 = outputs[12];
    U5_DB1 = outputs[13];
    U5_DB2 = outputs[14];
    U5_DB3 = outputs[15];
    U5_DB4 = outputs[16];
    U5_DB5 = outputs[17];
    U5_DB6 = outputs[18];
    U5_DB7 = outputs[19];

    U5_INV_CONVST = outputs[20];
    U5_INV_EOC    = outputs[21];
    U5_INV_RD     = outputs[22];

    U12_DB0 = outputs[23];
    U12_DB1 = outputs[24];
    U12_DB2 = outputs[25];
    U12_DB3 = outputs[26];
    U12_DB4 = outputs[27];
    U12_DB5 = outputs[28];
    U12_DB6 = outputs[29];
    U12_DB7 = outputs[30];

    U12_INV_CONVST = outputs[31];
    U12_INV_EOC    = outputs[32];
    U12_INV_RD     = outputs[33];

    U20_DB0 = outputs[34];
    U20_DB1 = outputs[35];
    U20_DB2 = outputs[36];
    U20_DB3 = outputs[37];
    U20_DB4 = outputs[38];
    U20_DB5 = outputs[39];
    U20_DB6 = outputs[40];
    U20_DB7 = outputs[41];

    U20_INV_CONVST = outputs[42];
    U20_INV_EOC    = outputs[43];
    U20_INV_RD     = outputs[44];

    U23_HIN = outputs[45];
    U23_LIN = outputs[46];
    U23_SD  = outputs[47];

    U24_HIN = outputs[48];
    U24_LIN = outputs[49];
    U24_SD  = outputs[50];
end


endmodule
