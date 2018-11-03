`timescale 1ns / 1ps
module deint #(
	parameter DW = 8,
	parameter I  = 12,
	parameter M  = 17
)(
	input				clk,
	input				rst_n,
	input				sym_ena,
	input	[DW-1:0]	sym_din,
	output	reg			sym_vld,
	output	[DW-1:0]	sym_dout
);

localparam	RAM_DEPTH = M*(I-1)*I/2 + 1;

reg								ena;
reg		[$clog2(I)-1:0]			icnt;
reg		[$clog2(RAM_DEPTH)-1:0]	ram_addr;
wire	[$clog2(RAM_DEPTH)-1:0]	deint_addr;

always @ (posedge clk, negedge rst_n)
	if(~rst_n)
		icnt <= 'd0;
	else begin
		if(icnt < I - 'd1 && sym_ena)
			icnt <= icnt + 'd1;
		else if(icnt == I - 'd1 && sym_ena)
			icnt <= 'd0;
	end

always @ (posedge clk, negedge rst_n)
	if(~rst_n)
	begin
		ena		<= 1'b0;
		sym_vld <= 1'b0;
	end
	else begin
		ena		<= sym_ena;
		sym_vld <= ena;
	end

// Deinterleaving Address Register
assign deint_addr = ram_addr + (I - 1 - icnt) * M;

always @ (posedge clk, negedge rst_n)
	if(~rst_n)
		ram_addr <= 'd0;
	else if(sym_ena)
		ram_addr <= deint_addr >= RAM_DEPTH ? deint_addr - RAM_DEPTH : deint_addr;

// Deinterleaving RAM
spram #(
	.DP(RAM_DEPTH),
	.DW(DW)
	) U_DEINT_RAM (
		.clk		(clk			),
		.wen		(~sym_ena		),
		.adr		(ram_addr		),
		.din		(sym_din		),
		.dout		(sym_dout		)
);

endmodule
