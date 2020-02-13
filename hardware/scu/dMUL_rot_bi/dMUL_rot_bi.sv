`define DATAWD 8

module dMUL_rot_bi (
    input logic clk,    // Clock
    input logic rst_n,  // Asynchronous reset active low
    input logic [`DATAWD-1:0] iA,
    input logic [`DATAWD-1:0] iB,
    input logic loadA,
    input logic loadB,
    output logic [`DATAWD*2-1:0] oC
);
    
    logic [`DATAWD-1:0] iA_buf;
    logic [`DATAWD-1:0] iB_buf;
    logic [`DATAWD-1:0] cntA;
    logic [`DATAWD-1:0] cntB;
    logic [`DATAWD-1:0] cntU;
    
    always_ff @(posedge clk or negedge rst_n) begin : proc_cntA
        if(~rst_n) begin
            cntA <= 0;
        end else begin
            if (loadA) begin
                cntA <= 0;
            end else begin
                cntA <= cntA + 1;
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin : proc_cntB
        if(~rst_n) begin
            cntB <= 0;
        end else begin
            if (loadB) begin
                cntB <= 0;
            end else begin
                cntB <= cntB + (cntU != 8'hFF);
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin : proc_cntU
        if(~rst_n) begin
            cntU <= 0;
        end else begin
            if (loadA) begin
                cntU <= 0;
            end else begin
                cntU <= cntU + 1;
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin : proc_iA_buf
        if(~rst_n) begin
            iA_buf <= 0;
        end else begin
            if(loadA) begin
                iA_buf <= iA;
            end else begin
                iA_buf <= iA_buf;
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin : proc_iB_buf
        if(~rst_n) begin
            iB_buf <= 0;
        end else begin
            if(loadB) begin
                iB_buf <= iB;
            end else begin
                iB_buf <= iB_buf;
            end
        end
    end

    always_ff @(posedge clk or negedge rst_n) begin : proc_oC
        if(~rst_n) begin
            oC <= 0;
        end else begin
            if (loadA) begin
                oC <= 0;
            end else begin
                if ((iA_buf > cntA) ^ (iB_buf > cntB)) begin
                    oC <= oC + 1;
                end else begin
                    oC <= oC;
                end
            end
        end
    end

endmodule