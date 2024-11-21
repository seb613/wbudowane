
module LAB01SYSWBUDOWANE (
	clk_clk,
	hex_export,
	leds_export,
	pushbutton_export,
	reset_reset_n,
	sw_sliders_export);	

	input		clk_clk;
	output	[47:0]	hex_export;
	output	[9:0]	leds_export;
	input	[3:0]	pushbutton_export;
	input		reset_reset_n;
	input	[9:0]	sw_sliders_export;
endmodule
