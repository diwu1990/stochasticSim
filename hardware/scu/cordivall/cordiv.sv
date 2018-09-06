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
    logic [SRDEPTH-1 : 0] SR;
    logic srout;

    assign srout = SR[sel];
    assign quotient = divisor ? dividend : srout;

    // <= SR[0] <= SR[1] <= quotient
    always_ff @(posedge clk or negedge rst_n) begin : proc_SR
        if(~rst_n) begin
            SR <= 0;
        end else begin
            if(divisor == 1) begin
                SR <= {SR[1], quotient};
            end
            else begin
                SR <= SR;
            end
        end
    end

endmodule