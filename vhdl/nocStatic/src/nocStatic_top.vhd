library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

use work.switchPkg.all;
use work.headerPkg.all;

entity top is
  port (
  	buttons		: in std_logic_vector(4 downto 0);
    sys_clk_p 	: in std_logic;
    sys_clk_n 	: in std_logic;
    
    ledIO 		: out std_logic_vector(4 downto 0)
  );
end top;



architecture rtl of top is
	
	signal clk125 : std_logic;
	signal clkGenLocked : std_logic;
	
	constant globalAddrSwitch0 : globalAddr := "0000";
	constant globalAddrSwitch1 : globalAddr := "0001";
	
	
	signal fb0_inputIn 	: inputLinkIn;
	signal fb0_inputOut	: inputLinkOut;
	signal fb0_outputIn	: outputLinkIn;
	signal fb0_outputOut: outputLinkOut;
	
	signal fb1_inputIn 	: inputLinkIn;
	signal fb1_inputOut	: inputLinkOut;
	signal fb1_outputIn	: outputLinkIn;
	signal fb1_outputOut: outputLinkOut;
	
	signal fb2_inputIn 	: inputLinkIn;
	signal fb2_inputOut	: inputLinkOut;
	signal fb2_outputIn	: outputLinkIn;
	signal fb2_outputOut: outputLinkOut;
	
	signal fb3_inputIn 	: inputLinkIn;
	signal fb3_inputOut	: inputLinkOut;
	signal fb3_outputIn	: outputLinkIn;
	signal fb3_outputOut: outputLinkOut;
	
	signal sw0_inputLinksIn 	: inputLinkInArray(numPorts-1 downto 0);
	signal sw0_inputLinksOut	: inputLinkOutArray(numPorts-1 downto 0);
	signal sw0_outputLinksIn	: outputLinkInArray(numPorts-1 downto 0);
	signal sw0_outputLinksOut	: outputLinkOutArray(numPorts-1 downto 0);
	
	signal sw1_inputLinksIn 	: inputLinkInArray(numPorts-1 downto 0);
	signal sw1_inputLinksOut	: inputLinkOutArray(numPorts-1 downto 0);
	signal sw1_outputLinksIn	: outputLinkInArray(numPorts-1 downto 0);
	signal sw1_outputLinksOut	: outputLinkOutArray(numPorts-1 downto 0);
		
	component interSwitchFifo
		port (
		clk: IN std_logic;
		rst: IN std_logic;
		din: IN std_logic_VECTOR(8 downto 0);
		wr_en: IN std_logic;
		rd_en: IN std_logic;
		dout: OUT std_logic_VECTOR(8 downto 0);
		full: OUT std_logic;
		empty: OUT std_logic
	);
	end component;
	
	component clk_gen_125Mhz
		port (
		  CLK_IN1_P         : in     std_logic;
		  CLK_IN1_N         : in     std_logic;
		  CLK_OUT1          : out    std_logic;
		  RESET             : in     std_logic;
		  LOCKED            : out    std_logic
		 );
	end component;
	
begin

	ledIO(0) <= clkGenLocked;

	clkGen125Mhz : clk_gen_125Mhz
		port map (
		    CLK_IN1_P	=> sys_clk_p,
		    CLK_IN1_N	=> sys_clk_n,
	    	CLK_OUT1	=> clk125,
	    	RESET		=> buttons(0),
	    	LOCKED		=> clkGenLocked
		);
    	
   	-----------------------------------------------------------------
	-- FUNCTIONAL BLOCK 0
	-----------------------------------------------------------------
	fb_0 : entity work.simpleFB
		generic map(
			headerValue => "00010000",
			resetWithToken => true
		)
		port map(
			clk			=> clk125,
			reset		=> clkGenLocked,
			led			=> ledIO(1),
			button		=> buttons(1),
			inputIn		=> fb0_inputIn,
			inputOut	=> fb0_inputOut,
			outputIn	=> fb0_outputIn,
			outputOut	=> fb0_outputOut
		);
		
	-- downstream fifo
	fifo_fb0_input : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw0_outputLinksOut(0).data,
			wr_en => sw0_outputLinksOut(0).writeEnable,
			rd_en => fb0_inputOut.readEnable,
			dout => fb0_inputIn.data,
			full => sw0_outputLinksIn(0).full,
			empty => fb0_inputIn.empty
		);
	
	-- upstream fifo
	fifo_fb0_output : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => fb0_outputOut.data,
			wr_en => fb0_outputOut.writeEnable,
			rd_en => sw0_inputLinksOut(0).readEnable,
			dout => sw0_inputLinksIn(0).data,
			full => fb0_outputIn.full,
			empty => sw0_inputLinksIn(0).empty
		);
		
	-----------------------------------------------------------------
	-- FUNCTIONAL BLOCK 1
	-----------------------------------------------------------------
	fb_1 : entity work.simpleFB
		generic map(
			headerValue => "00000001",
			resetWithToken => false
		)
		port map(
			clk			=> clk125,
			reset		=> clkGenLocked,
			led			=> ledIO(2),
			button		=> buttons(2),
			inputIn		=> fb1_inputIn,
			inputOut	=> fb1_inputOut,
			outputIn	=> fb1_outputIn,
			outputOut	=> fb1_outputOut
		);
		
	-- downstream fifo
	fifo_fb1_input : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw0_outputLinksOut(1).data,
			wr_en => sw0_outputLinksOut(1).writeEnable,
			rd_en => fb1_inputOut.readEnable,
			dout => fb1_inputIn.data,
			full => sw0_outputLinksIn(1).full,
			empty => fb1_inputIn.empty
		);
	
	-- upstream fifo
	fifo_fb1_output : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => fb1_outputOut.data,
			wr_en => fb1_outputOut.writeEnable,
			rd_en => sw0_inputLinksOut(1).readEnable,
			dout => sw0_inputLinksIn(1).data,
			full => fb1_outputIn.full,
			empty => sw0_inputLinksIn(1).empty
		);
		
		
	-----------------------------------------------------------------
	-- FUNCTIONAL BLOCK 2
	-----------------------------------------------------------------
	fb_2 : entity work.simpleFB
		generic map(
			headerValue => "00010001",
			resetWithToken => false
		)
		port map(
			clk			=> clk125,
			reset		=> clkGenLocked,
			led			=> ledIO(3),
			button		=> buttons(3),
			inputIn		=> fb2_inputIn,
			inputOut	=> fb2_inputOut,
			outputIn	=> fb2_outputIn,
			outputOut	=> fb2_outputOut
		);
		
	-- downstream fifo
	fifo_fb2_input : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw1_outputLinksOut(0).data,
			wr_en => sw1_outputLinksOut(0).writeEnable,
			rd_en => fb2_inputOut.readEnable,
			dout => fb2_inputIn.data,
			full => sw1_outputLinksIn(0).full,
			empty => fb2_inputIn.empty
		);
	
	-- upstream fifo
	fifo_fb2_output : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => fb2_outputOut.data,
			wr_en => fb2_outputOut.writeEnable,
			rd_en => sw1_inputLinksOut(0).readEnable,
			dout => sw1_inputLinksIn(0).data,
			full => fb2_outputIn.full,
			empty => sw1_inputLinksIn(0).empty
		);
		
	
	-----------------------------------------------------------------
	-- FUNCTIONAL BLOCK 3
	-----------------------------------------------------------------
	fb_3 : entity work.simpleFB
		generic map(
			headerValue => "00000000",
			resetWithToken => false
		)
		port map(
			clk			=> clk125,
			reset		=> clkGenLocked,
			led			=> ledIO(4),
			button		=> buttons(4),
			inputIn		=> fb3_inputIn,
			inputOut	=> fb3_inputOut,
			outputIn	=> fb3_outputIn,
			outputOut	=> fb3_outputOut
		);
		
	-- downstream fifo
	fifo_fb3_input : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw1_outputLinksOut(1).data,
			wr_en => sw1_outputLinksOut(1).writeEnable,
			rd_en => fb3_inputOut.readEnable,
			dout => fb3_inputIn.data,
			full => sw1_outputLinksIn(1).full,
			empty => fb3_inputIn.empty
		);
	
	-- upstream fifo
	fifo_fb3_output : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => fb3_outputOut.data,
			wr_en => fb3_outputOut.writeEnable,
			rd_en => sw1_inputLinksOut(1).readEnable,
			dout => sw1_inputLinksIn(1).data,
			full => fb3_outputIn.full,
			empty => sw1_inputLinksIn(1).empty
		);
	
	
	
	
	
	-----------------------------------------------------------------
	-- SWITCHES
	-----------------------------------------------------------------
	
	sw_0 : entity work.switch
		generic map(
			globalAddress => globalAddrSwitch0
		)
		port map(
			clk				=> clk125,
			reset			=> clkGenLocked,
			inputLinksIn	=> sw0_inputLinksIn,
			inputLinksOut	=> sw0_inputLinksOut,
			outputLinksIn	=> sw0_outputLinksIn,
			outputLinksOut	=> sw0_outputLinksOut
		);

	sw_1 : entity work.switch
		generic map(
			globalAddress => globalAddrSwitch1
		)
		port map(
			clk				=> clk125,
			reset			=> clkGenLocked,
			inputLinksIn	=> sw1_inputLinksIn,
			inputLinksOut	=> sw1_inputLinksOut,
			outputLinksIn	=> sw1_outputLinksIn,
			outputLinksOut	=> sw1_outputLinksOut
		);
		
		
	-----------------------------------------------------------------
	-- INTER SWITCH FIFOS
	-----------------------------------------------------------------
	
	fifo_sw0ToSw1_0 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw0_outputLinksOut(2).data,
			wr_en => sw0_outputLinksOut(2).writeEnable,
			rd_en => sw1_inputLinksOut(2).readEnable,
			dout => sw1_inputLinksIn(2).data,
			full => sw0_outputLinksIn(2).full,
			empty => sw1_inputLinksIn(2).empty
		);
		
	fifo_sw0ToSw1_1 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw0_outputLinksOut(3).data,
			wr_en => sw0_outputLinksOut(3).writeEnable,
			rd_en => sw1_inputLinksOut(3).readEnable,
			dout => sw1_inputLinksIn(3).data,
			full => sw0_outputLinksIn(3).full,
			empty => sw1_inputLinksIn(3).empty
		);
	
	fifo_sw0ToSw1_2 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw0_outputLinksOut(4).data,
			wr_en => sw0_outputLinksOut(4).writeEnable,
			rd_en => sw1_inputLinksOut(4).readEnable,
			dout => sw1_inputLinksIn(4).data,
			full => sw0_outputLinksIn(4).full,
			empty => sw1_inputLinksIn(4).empty
		);
		
	fifo_sw0ToSw1_3 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw0_outputLinksOut(5).data,
			wr_en => sw0_outputLinksOut(5).writeEnable,
			rd_en => sw1_inputLinksOut(5).readEnable,
			dout => sw1_inputLinksIn(5).data,
			full => sw0_outputLinksIn(5).full,
			empty => sw1_inputLinksIn(5).empty
		);
		
	
	
	fifo_sw1Tosw0_0 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw1_outputLinksOut(2).data,
			wr_en => sw1_outputLinksOut(2).writeEnable,
			rd_en => sw0_inputLinksOut(2).readEnable,
			dout => sw0_inputLinksIn(2).data,
			full => sw1_outputLinksIn(2).full,
			empty => sw0_inputLinksIn(2).empty
		);
		
	fifo_sw1Tosw0_1 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw1_outputLinksOut(3).data,
			wr_en => sw1_outputLinksOut(3).writeEnable,
			rd_en => sw0_inputLinksOut(3).readEnable,
			dout => sw0_inputLinksIn(3).data,
			full => sw1_outputLinksIn(3).full,
			empty => sw0_inputLinksIn(3).empty
		);
	
	fifo_sw1Tosw0_2 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw1_outputLinksOut(4).data,
			wr_en => sw1_outputLinksOut(4).writeEnable,
			rd_en => sw0_inputLinksOut(4).readEnable,
			dout => sw0_inputLinksIn(4).data,
			full => sw1_outputLinksIn(4).full,
			empty => sw0_inputLinksIn(4).empty
		);
		
	fifo_sw1Tosw0_3 : interSwitchFifo
		port map (
			clk => clk125,
			rst => clkGenLocked,
			din => sw1_outputLinksOut(5).data,
			wr_en => sw1_outputLinksOut(5).writeEnable,
			rd_en => sw0_inputLinksOut(5).readEnable,
			dout => sw0_inputLinksIn(5).data,
			full => sw1_outputLinksIn(5).full,
			empty => sw0_inputLinksIn(5).empty
		);
		
end architecture rtl;