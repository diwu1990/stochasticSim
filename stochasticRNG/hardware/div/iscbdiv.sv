module iscbdiv (
    input clk,    // Clock
    // input clk_en, // Clock Enable
    input rst_n,  // Asynchronous reset active low
    input dividend,
    input divisor,
    output quot
);

    // in-stream correlation-based divisor
    // dividend / divisor = quot
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

    

endmodule