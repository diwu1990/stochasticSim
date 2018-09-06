module cordiv (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    // control port
    input sel,
    // data port
    input dividend,
    input divisor,
    output quotient
);
    // define the depth of shift register
    // 2 is recommended for accuracy according to simuation
    parameter SRDEPTH = 2;

    // shift register
    logic [SRDEPTH-1 : 0] shiftReg;
    logic srout;

    assign srout = shiftReg[sel];
    assign quotient = divisor ? dividend : srout;

    // <= shiftReg[0] <= shiftReg[1] <= quotient
    always_ff @(posedge clk or negedge rst_n) begin : proc_SR
        if(~rst_n) begin
            shiftReg <= 0;
        end else begin
            if(divisor == 1) begin
                shiftReg <= {shiftReg[1], quotient};
            end
            else begin
                shiftReg <= shiftReg;
            end
        end
    end

endmodule