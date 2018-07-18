module add2 (
	input sel,
	input [1:0] in,
	output out
);

	assign out = sel ? in[1] : in[0];

endmodule