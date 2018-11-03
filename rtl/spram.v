`timescale 1ns/1ps
module spram #(
	parameter DP = 8,
	parameter DW = 8
)(
	input						clk,
	input						wen,
	input	[$clog2(DP)-1:0]	adr,
	input	[DW-1:0]			din,
	output	[DW-1:0]			dout
);

reg		[DW-1:0]	dq;
reg		[DW-1:0]	mem[0:DP-1];

always @ (posedge clk)
	if(~wen)
		mem[adr] <= din;

always @ (posedge clk)
	dq <= mem[adr];

assign dout = dq;

endmodule
