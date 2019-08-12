`define INWD 8

module bMUL (
    input logic clk,    // Clock
    input logic rst_n,  // Asynchronous reset active low
    input logic [`INWD-1:0] iA,
    input logic [`INWD-1:0] iB,
    output logic [`INWD*2-1:0] oC,
);
    logic [`INWD-1:0] iA_buf;
    logic [`INWD-1:0] iB_buf;

    always_ff @(posedge clk or negedge rst_n) begin : proc_oC
        if(~rst_n) begin
            oC <= 0;
        end else begin
            oC <= iA * iB;
        end
    end

endmodule