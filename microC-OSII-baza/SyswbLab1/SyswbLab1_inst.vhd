	component SyswbLab1 is
		port (
			clk_clk           : in  std_logic                     := 'X';             -- clk
			hex_export        : out std_logic_vector(47 downto 0);                    -- export
			leds_export       : out std_logic_vector(9 downto 0);                     -- export
			pushbutton_export : in  std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			reset_reset_n     : in  std_logic                     := 'X';             -- reset_n
			sw_sliders_export : in  std_logic_vector(9 downto 0)  := (others => 'X')  -- export
		);
	end component SyswbLab1;

	u0 : component SyswbLab1
		port map (
			clk_clk           => CONNECTED_TO_clk_clk,           --        clk.clk
			hex_export        => CONNECTED_TO_hex_export,        --        hex.export
			leds_export       => CONNECTED_TO_leds_export,       --       leds.export
			pushbutton_export => CONNECTED_TO_pushbutton_export, -- pushbutton.export
			reset_reset_n     => CONNECTED_TO_reset_reset_n,     --      reset.reset_n
			sw_sliders_export => CONNECTED_TO_sw_sliders_export  -- sw_sliders.export
		);

