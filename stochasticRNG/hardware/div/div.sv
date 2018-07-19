module div (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input [5:0]randNum,
    input dividend,
    input divisor,
    output quotient
);
    
    logic [5:0] cnt;
    logic inc;
    logic dec;

    always_ff @(posedge clk or negedge rst_n) begin : proc_cnt
        if(~rst_n) begin
            cnt <= 6'b100000;
        end else begin
            if(inc & ~dec & ~&cnt) begin
                cnt <= cnt + 1;
            end else if(~inc & dec & ~|cnt) begin
                cnt <= cnt - 1;
            end else begin
                cnt <= cnt;
            end
        end
    end

    assign quotient = cnt >= randNum;
    always_ff @(posedge clk or negedge rst_n) begin : proc_out_d1
        if(~rst_n) begin
            out_d1 <= 0;
        end else begin
            out_d1 <= quotient;
        end
    end

    assign inc = dividend;
    assign dec = quotient & divisor;

endmodule
