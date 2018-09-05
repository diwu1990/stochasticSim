module cordiv (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input dividend,
    input divisor,
    input sel,
    output quotient
);
    // define the depth of shift register
    // 2 is recommended for accuracy according to simuation
    parameter SRDEPTH = 2;

    // shift register
    logic [SRDEPTH-1 : 0] SR;
    logic srout;

    assign srout = SR[sel];

    assign quotient = divisor ? dividend : srout;

endmodule