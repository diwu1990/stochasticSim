`timescale 1ns/1ns
`include "andMUL.sv"

module andMUL_tb ();

    logic   clk;
    logic   rst_n;
    logic   [`INWD-1:0] iA;
    logic   [`INWD-1:0] iB;
    logic   loadA;
    logic   loadB;
    logic   oC;

    andMUL U_andMUL(
        .clk(clk),    // Clock
        .rst_n(rst_n),  // Asynchronous reset active low
        .iA(iA),
        .iB(iB),
        .loadA(loadA),
        .loadB(loadB),
        .oC(oC)
        );

    // clk define
    always #5 clk = ~clk;

    `ifdef DUMPFSDB
        initial begin
            $fsdbDumpfile("andMUL.fsdb");
            $fsdbDumpvars(0,"+all");
            // $fsdbDumpvars;
        end
    `endif


    initial
    begin
        clk = 1;
        rst_n = 0;
        iA = 128;
        iB = 128;
        loadA = 0;
        loadB = 0;

        #15;
        rst_n = 1;

        #10;
        loadA = 1;
        loadB = 1;

        #10;
        loadA = 0;
        loadB = 0;

        #1000;
        $finish;
    end

endmodule