`include "SobolRNGDim1_4b.sv"
`include "SobolRNGDim1_8b.sv"
`include "muxADD.sv"
`include "gMUL_bi.sv"

module gainMAC_bi_scaled (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input logic [7:0] iA [15:0],
    input logic [7:0] iB [15:0],
    input logic loadA,
    input logic loadB,
    output oC
);
    logic [7:0] sobolSeqA;
    logic [7:0] sobolSeqB;
    logic [15:0] mulC;
    
    SobolRNGDim1_8b U_SobolRNGDim1_8b_A(
        .clk(clk),
        .rst_n(rst_n),
        .enable(1'b1),
        .sobolSeq(sobolSeqA)
        );

    SobolRNGDim1_8b U_SobolRNGDim1_8b_B(
        .clk(clk),
        .rst_n(rst_n),
        .enable(1'b1),
        .sobolSeq(sobolSeqB)
        );

    genvar i;
    generate
        for (i = 0; i < 16; i++) begin
            gMUL_bi U_gMUL_bi(
                .clk(clk),    // Clock
                .rst_n(rst_n),  // Asynchronous reset active low
                .iA(iA[i]),
                .iB(iB[i]),
                .loadA(loadA),
                .loadB(loadB),
                .sobolSeqA(sobolSeqA),
                .sobolSeqB(sobolSeqB),
                .oC(mulC[i])
                );
        end
    endgenerate

    logic [3:0] sel;
    SobolRNGDim1_4b U_SobolRNGDim1_4b(
        .clk(clk),
        .rst_n(rst_n),
        .enable(1'b1),
        .sobolSeq(sel)
        );

    muxADD U_muxADD(
        .clk(clk),    // Clock
        .rst_n(rst_n),  // Asynchronous reset active low
        .in(mulC),
        .sel(sel),
        .out(oC)
        );

endmodule