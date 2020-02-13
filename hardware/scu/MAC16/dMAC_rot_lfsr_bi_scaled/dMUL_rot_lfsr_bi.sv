`define DATAWD 8
`include "lfsr.sv"

module dMUL_rot_lfsr_bi (
    input logic clk,    // Clock
    input logic rst_n,  // Asynchronous reset active low
    input logic [`DATAWD-1:0] iA,
    input logic [`DATAWD-1:0] iB,
    input logic loadA,
    input logic loadB,
    input logic [`DATAWD-1:0] iseedA,
    input logic [`DATAWD-1:0] iseedB,
    input logic [`DATAWD-1:0] iseedU,
    // output logic [`DATAWD*2-1:0] oC
    output logic oC
);
    
    logic [`DATAWD-1:0] iA_buf;
    logic [`DATAWD-1:0] iB_buf;
    logic [`DATAWD-1:0] cntA;
    logic [`DATAWD-1:0] cntB;
    logic [`DATAWD-1:0] cntU;
    
    logic u_done;

    lfsr #(.NUM_BITS(`DATAWD)) LFSR_inst_A
        (.clk(clk),
        .enable(1'b1),
        .i_Seed_DV(loadA),
        .i_Seed_Data(iseedA), // Replication
        .o_LFSR_Data(cntA),
        .o_LFSR_Done()
        );

    lfsr #(.NUM_BITS(`DATAWD)) LFSR_inst_B
        (.clk(clk),
        .enable(~u_done),
        .i_Seed_DV(loadB),
        .i_Seed_Data(iseedB), // Replication
        .o_LFSR_Data(cntB),
        .o_LFSR_Done()
        );

    lfsr #(.NUM_BITS(`DATAWD)) LFSR_inst_U
        (.clk(clk),
        .enable(1'b1),
        .i_Seed_DV(loadA),
        .i_Seed_Data(iseedU), // Replication
        .o_LFSR_Data(cntU),
        .o_LFSR_Done(u_done)
        );

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

    // always_ff @(posedge clk or negedge rst_n) begin : proc_oC
    //     if(~rst_n) begin
    //         oC <= 0;
    //     end else begin
    //         if (loadA) begin
    //             oC <= 0;
    //         end else begin
    //             if ((iA_buf > cntA) ^ (iB_buf > cntB)) begin
    //                 oC <= oC + 1;
    //             end else begin
    //                 oC <= oC;
    //             end
    //         end
    //     end
    // end

    assign oC = (iA_buf > cntA) ^ (iB_buf > cntB)

endmodule