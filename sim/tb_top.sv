`timescale 1ns / 1ps
module tb_top();

integer	i, f;

logic			clk;
logic			rst_n;

logic	[7:0]	sym_itlv[12*11*17*204];
logic			sym_ena;
logic	[7:0]	sym_din;
logic			sym_vld;
logic	[7:0]	sym_dout;

initial forever #5 clk = ~clk;

deint U_DEINT (
	.clk		(clk		),
	.rst_n		(rst_n		),
	.sym_ena	(sym_ena	),
	.sym_din	(sym_din	),
	.sym_vld	(sym_vld	),
	.sym_dout	(sym_dout	)
);

initial begin
	clk = 1'b1;
	rst_n = 1'b0;
	sym_ena = 1'b0;
	#105ns;
	rst_n = 1'b1;
	#10ns;

	// Read Test Data
	i = 0;
	f = $fopen("test.dat", "r");
	while(!$feof(f))
	begin
		void'($fscanf(f, "%d", sym_itlv[i]));
		// $display("Value = %d", sym_itlv[i]);
		i = i + 1;
	end
	$fclose(f);

	foreach(sym_itlv[k])
	begin
		@(posedge clk);
		#1ns;
		sym_ena <= 1'b1;
		sym_din <= sym_itlv[k];
		@(posedge clk);
		#1ns;
		sym_ena <= 1'b0;
		sym_din <= 0;
		@(posedge clk);
	end
	#100ns;
	$finish;
end

initial begin
	$vcdplusfile("wave.vpd");
	$vcdpluson(0);
	$vcdplusmemon(0);
end

endmodule

