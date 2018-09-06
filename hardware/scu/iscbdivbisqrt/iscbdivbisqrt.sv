`include "cordiv.sv"

module iscbdivbisqrt (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input sel,
    input in,
    output out
);
    
    logic sel;
    logic [1:0] mux;
    logic dff;
    logic inv;
    logic srout;

    assign inv = ~dff;

    always_ff @(posedge clk or negedge rst_n) begin : proc_dff
        if(~rst_n) begin
            dff <= 0;
        end else begin
            dff <= ~dff;
        end
    end

    assign mux[0] = 1;
    assign mux[1] = in;
    assign out = sel ? mux[1] : mux[0];
    assign sel = srout;

    cordiv U_cordiv(
        .clk(clk),
        .rst_n(rst_n),
        .sel(sel)
        .dividend(dff),
        .divisor((inv & out) | dff),
        .quotient(),
        .srout(srout)
        );

endmodule
