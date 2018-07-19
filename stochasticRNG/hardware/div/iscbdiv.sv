`include "skewedSync.sv"

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

    logic [1:0] mux;
    logic sel;

    assign sel = divIn[1];
    assign mux[1] = divIn[0];
    assign quotient = sel ? mux[1] : mux[0];

    logic [1:0] traceReg;

    assign mux[0] = randNum ? traceReg[1] : traceReg[0];

    always_ff @(posedge clk or negedge rst_n) begin : proc_traceReg
        if(~rst_n) begin
            traceReg <= 0;
        end else begin
            traceReg <= divIn[1] ? {quotient, traceReg[1]} : traceReg;
        end
    end

endmodule
