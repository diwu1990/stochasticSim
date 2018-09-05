module skewedSync (
    input clk,    // Clock
    input rst_n,  // Asynchronous reset active low
    input [1:0] in,
    output [1:0] out
);
    parameter DEPTH = 4;
    // bit array of index 1 has a higher value than that of index 0

    logic [DEPTH - 1 : 0] cnt;

    assign out[1] = in[1];
    always_comb begin : proc_out0
        if((in[1] & ~in[0] & ~|cnt) | (in[1] & in[0])) begin
            out[0] = 1;
        end else begin
            out[0] <= 0;
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin : proc_cnt
        if(~rst_n) begin
            cnt <= 0;
        end else begin
            if(in[1] & ~in[0] & ~|cnt) begin
                cnt = cnt - 1;
            end else if(~in[1] & ~&cnt) begin
                cnt <= cnt + in[0];
            end else begin
                cnt <= cnt;
            end
        end
    end

endmodule