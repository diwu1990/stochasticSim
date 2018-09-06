`include "skewedSync.sv"
`include "cordiv.sv"

module iscbdiv (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input randNum,
    input dividend,
    input divisor,
    output quotient
);
    
    // in-stream correlation-based divisor
    // dividend / divisor = quotient
    // divisor is always larger than dividend

    logic [1:0] divIn;

    // input is send to a skewed synchronizer first.
    skewedSync #(
        .DEPTH(4)
        )
    U_skewedSync(
        .clk(clk),
        .rst_n(rst_n),
        .in({divisor,dividend}),
        .out(divIn)
        );

    cordiv U_cordiv(
        .clk(clk),
        .rst_n(rst_n),
        .sel(sel),
        .dividend(dividend_corr),
        .divisor(divisor_corr),
        .quotient(quotient)
        );

endmodule
