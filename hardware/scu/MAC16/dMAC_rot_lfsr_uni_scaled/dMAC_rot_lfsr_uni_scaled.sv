`include "SobolRNGDim1_4b.sv"
`include "muxADD.sv"
`include "dMUL_rot_lfsr_uni.sv"

module dMAC_rot_lfsr_uni_scaled (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input logic [7:0] iA [15:0],
    input logic [7:0] iB [15:0],
    input logic loadA,
    input logic loadB,
    input logic [7:0] iseedA [15:0],
    input logic [7:0] iseedB [15:0],
    input logic [7:0] iseedU [15:0],
    output oC
);
    logic [15:0] mulC;

    genvar i;
    generate
        for (i = 0; i < 16; i++) begin
            dMUL_rot_lfsr_uni U_dMUL_uni(
                .clk(clk),    // Clock
                .rst_n(rst_n),  // Asynchronous reset active low
                .iA(iA[i]),
                .iB(iB[i]),
                .loadA(loadA),
                .loadB(loadB),
                .iseedA(iseedA[i]),
                .iseedB(iseedB[i]),
                .iseedU(iseedU[i]),
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