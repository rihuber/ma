library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.switchPkg.all;

entity simpleFB is
	generic(
		headerValue		: std_logic_vector(7 downto 0) := "00010001";
		resetWithToken	: boolean := false
	);
	port (
		clk 		: in std_logic;
		reset 		: in std_logic;
		
		led 		: out std_logic;
		button		: in std_logic;
		
		inputIn		: in inputLinkIn;
		inputOut	: out inputLinkOut;
		outputIn	: in outputLinkIn;
		outputOut	: out outputLinkOut
	);
end entity simpleFB;

architecture rtl of simpleFB is

	constant counterWidth : integer := 28;
	
	type state is (IDLE, RECEIVING_TOKEN, HOLDING_TOKEN, SENDING_TOKEN);
	signal state_p, state_n : state;
	signal counter_p, counter_n	: unsigned(counterWidth-1 downto 0);
	
	constant counterMaxValue : unsigned(counterWidth-1 downto 0) := (others => '1');
	constant counterMinValue : unsigned(counterWidth-1 downto 0) := (others => '0');
	
	signal dataValue_p, dataValue_n : std_logic_vector(7 downto 0);
	
begin

	nomem_output : process (state_p, counter_p, dataValue_p)
	begin
		led  <= '0';
		inputOut.readEnable <= '0';
		outputOut.writeEnable <= '0';
		outputOut.data <= (others => '-');
		
		if state_p = RECEIVING_TOKEN then
				inputOut.readEnable <= '1';
		elsif state_p = HOLDING_TOKEN then
				led <= '1';
		elsif state_p = SENDING_TOKEN then 
			outputOut.writeEnable <= '1';
			if counter_p = counterMaxValue then
				outputOut.data(7 downto 0) <= headerValue;
			else
				outputOut.data(7 downto 0) <= dataValue_p;
			end if;
			if counter_p = counterMinValue then
				outputOut.data(8) <= '1';
			else
				outputOut.data(8) <= '0';
			end if;
		end if;
		
	end process nomem_output;

	nomem_nextState : process (state_p, counter_p, inputIn, button, outputIn, dataValue_p)
		--variable controlBitVar: std_logic := '0';
	begin
		state_n <= state_p;
		counter_n <= counter_p;
		dataValue_n <= dataValue_p;
		
		case state_p is
			when IDLE =>
				if inputIn.empty = '0' then
					state_n <= RECEIVING_TOKEN;
				end if;
			when RECEIVING_TOKEN =>
				if inputIn.empty = '0' then
					dataValue_n <= inputIn.data(7 downto 0);
					if inputIn.data(8) = '1' then
						state_n <= HOLDING_TOKEN;
					end if;
				end if;
			when HOLDING_TOKEN =>
				if button = '1' then
					state_n <= SENDING_TOKEN;
				end if;
			when SENDING_TOKEN =>
				if outputIn.full = '0' then
					if counter_p = counterMinValue then
						state_n <= idle;
						counter_n <= (others => '1');
					else
						counter_n <= counter_p - 1;
					end if;
				end if;
		end case;
	end process nomem_nextState;

	mem_stateTransition : process (reset, clk)
	begin
		if reset = '0' then
			counter_p <= (others => '1');
			dataValue_p <= "01010101";
			if resetWithToken then
				state_p <= HOLDING_TOKEN;
			else
				state_p <= IDLE;
			end if;
		elsif rising_edge(clk) then
			state_p <= state_n;
			counter_p <= counter_n;
			dataValue_p <= dataValue_n;
		end if;
	end process mem_stateTransition;

end architecture rtl;
