module hvps_controller(
    input  wire clk,

    output reg U12_INV_EOC, U12_INV_RD, U12_INV_CS, U12_INV_CONVST,
    output reg U12_DB0, U12_DB1, U12_DB2, U12_DB3, U12_DB4, U12_DB5, U12_DB6, U12_DB7,

    output reg U5_INV_EOC, U5_INV_RD, U5_INV_CS, U5_INV_CONVST,
    output reg U5_DB0, U5_DB1, U5_DB2, U5_DB3, U5_DB4, U5_DB5, U5_DB6, U5_DB7,

    output reg U20_INV_EOC, U20_INV_RD, U20_INV_CS, U20_INV_CONVST,
    output reg INV_PD,
    output reg U20_DB0, U20_DB1, U20_DB2, U20_DB3, U20_DB4, U20_DB5, U20_DB6, U20_DB7,

    output reg OR4, OR5, OR6, OT4, OT5,

    output reg ESP_IN_SHIFT, ESP_IN_VALUE, ESP_STATE_TO_OUTPUT,
    output reg ESP_OUT_SHIFT, ESP_OUT_VALUE, ESP_GO_LIVE, ESP_EMERGENCY_SHUTDOWN
);

    // ------------------------------------------------------------
    // Internal state
    // ------------------------------------------------------------
    reg [31:0] counters [0:60];
    reg [60:0] outputs;

    // ------------------------------------------------------------
    // Safe, wide frequency table (Hz)
    // ------------------------------------------------------------
   localparam [16*61-1:0] PIN_FREQ = {
    16'd39, 16'd43, 16'd58, 16'd54, 16'd30, 16'd52, 16'd50,
    16'd1000,  16'd3000,  16'd2000,
    16'd144, 16'd142, 16'd126, 16'd128, 16'd135, 16'd141, 16'd143,
    16'd124, 16'd120, 16'd113, 16'd111, 16'd106, 16'd104, 16'd100,
    16'd72, 16'd70, 16'd68, 16'd64, 16'd59,
    16'd74, 16'd84, 16'd85, 16'd83, 16'd75, 16'd73, 16'd71,
    16'd28, 16'd129, 16'd127, 16'd125, 16'd121,
    16'd38, 16'd42, 16'd44, 16'd49, 16'd51, 16'd53, 16'd55, 16'd112
};


    // ------------------------------------------------------------
    // Power-up initialisation (FPGA-safe)
    // ------------------------------------------------------------
    integer k;
    initial begin
        for (k = 0; k < 61; k = k + 1) begin
            counters[k] = 0;
            outputs[k]  = 0;
        end
    end

    // ------------------------------------------------------------
    // Clocked toggle engine
    // ------------------------------------------------------------
    integer j;
    always @(posedge clk) begin
        for (j = 0; j < 61; j = j + 1) begin
            if (counters[j] >= (50_000_000 / (2 * PIN_FREQ[16*j +: 16]))) begin
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
        INV_PD            = outputs[0];

        U20_INV_EOC       = outputs[1];
        U20_INV_RD        = outputs[2];
        U20_INV_CS        = outputs[3];
        U20_INV_CONVST    = outputs[4];

        {U20_DB7,U20_DB6,U20_DB5,U20_DB4,
         U20_DB3,U20_DB2,U20_DB1,U20_DB0} = outputs[12:5];

        U12_INV_EOC       = outputs[13];
        U12_INV_RD        = outputs[14];
        U12_INV_CS        = outputs[15];
        U12_INV_CONVST    = outputs[16];

        {U12_DB7,U12_DB6,U12_DB5,U12_DB4,
         U12_DB3,U12_DB2,U12_DB1,U12_DB0} = outputs[24:17];

        U5_INV_EOC        = outputs[25];
        U5_INV_RD         = outputs[26];
        U5_INV_CS         = outputs[27];
        U5_INV_CONVST     = outputs[28];

        {U5_DB7,U5_DB6,U5_DB5,U5_DB4,
         U5_DB3,U5_DB2,U5_DB1,U5_DB0}     = outputs[36:29];

        OR4               = outputs[37];
        OR5               = outputs[38];
        OR6               = outputs[39];
        OT4               = outputs[40];
        OT5               = outputs[41];

        ESP_IN_SHIFT          = outputs[42];
        ESP_IN_VALUE          = outputs[43];
        ESP_STATE_TO_OUTPUT   = outputs[44];
        ESP_OUT_SHIFT         = outputs[45];
        ESP_OUT_VALUE         = outputs[46];
        ESP_GO_LIVE           = outputs[47];
        ESP_EMERGENCY_SHUTDOWN= outputs[48];
    end

endmodule
