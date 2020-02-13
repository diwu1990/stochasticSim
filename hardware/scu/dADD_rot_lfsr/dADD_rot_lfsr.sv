`include "SobolRNGDim1.sv"
`include "muxADD.sv"

module dADD_rot_lfsr (
    input logic clk,    // Clock
    input logic rst_n,  // Asynchronous reset active low
    input logic [`INUM-1:0] in,
    input logic loadA,
    input logic [2:0] iseedA,
    output logic out
);
    logic [`LOGINUM-1:0] sel;

    logic [`LOGINUM-1:0] cnt;

    lfsr #(.NUM_BITS(`DATAWD)) LFSR_inst_A
        (.clk(clk),
        .enable(1'b1),
        .i_Seed_DV(loadA),
        .i_Seed_Data(iseedA), // Replication
        .o_LFSR_Data(cnt),
        .o_LFSR_Done()
        );

    SobolRNGDim1 U_SobolRNGDim1(
        .clk(clk),
        .rst_n(rst_n),
        .enable(cnt == 1),
        .sobolSeq(sel)
        );

    muxADD U_muxADD(
        .clk(clk),    // Clock
        .rst_n(rst_n),  // Asynchronous reset active low
        .in(in),
        .sel(sel),
        .out(out)
        );

endmodule