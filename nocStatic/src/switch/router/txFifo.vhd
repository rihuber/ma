library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.utilPkg.all;
use work.headerPkg.all;
use work.switchPkg.all;

entity txFifo is
	port (
		clk 		: in std_logic;
		reset 		: in std_logic;
		
		writeEnable	: in std_logic;
		readEnable	: in std_logic;
		empty		: out std_logic;
		
		rxPortNrIn	: in portNr;
		rxPortNrOut	: out portNr	
	);
end entity txFifo;

architecture rtl of txFifo is

	subtype position is unsigned(toLog2Ceil(numPorts-1) downto 0);
	subtype positionInteger is integer range numPorts-1 downto 0;
	function positionToInteger(pos: position) return positionInteger is
		variable result : positionInteger;
	begin
		result := to_integer(pos);
		return result;
	end function positionToInteger;
	
	signal readPosition_p, readPosition_n, writePosition_p, writePosition_n : position;
	
	signal fillCount_p, fillCount_n : position;
	
	signal ringBuffer_p, ringBuffer_n : portNrArray(numPorts-1 downto 0);
	
begin

	rxPortNrOut <= ringBuffer_p(positionToInteger(readPosition_p));

	nomem_output : process(fillCount_p) is
	begin
		empty <= '0';
		if fillCount_p = 0 then
			empty <= '1';
		end if;
	end process nomem_output;
 
 	nomem_nextState : process(readPosition_p, writePosition_p, fillCount_p, ringBuffer_p, writeEnable, readEnable, rxPortNrIn) is
 	begin
 		-- default assignments
 		readPosition_n <= readPosition_p;
 		writePosition_n <= writePosition_p;
 		fillCount_n <= fillCount_p;
 		ringBuffer_n <= ringBuffer_p;
 		
 		-- writing
 		if writeEnable = '1' then
 			ringBuffer_n(positionToInteger(writePosition_p)) <= rxPortNrIn;
 			if writePosition_p+1 < numPorts then
 				writePosition_n <= writePosition_p + 1;
 			else
 				writePosition_n <= (others => '0');
 			end if;
 			if readEnable = '0' then
 				fillCount_n <= fillCount_p + 1;
 			end if;
 		end if;
 		
 		-- reading
 		if readEnable = '1' and fillCount_p > 0 then
 			if readPosition_p+1 < numPorts then
 				readPosition_n <= readPosition_p + 1;
 			else
 				readPosition_n <= (others => '0');
 			end if;
 			if writeEnable = '0' then
 				fillCount_n <= fillCount_p-1;
 			end if;
 		end if;
 	end process nomem_nextState;

	mem_stateTransition : process (clk, reset) is
	begin
		if reset = '0' then
			readPosition_p <= (others => '0');
			writePosition_p <= (others => '0');
			fillCount_p <= (others => '0');
		elsif rising_edge(clk) then
			readPosition_p <= readPosition_n;
			writePosition_p <= writePosition_n;
			fillCount_p <= fillCount_n;
		end if;
	end process mem_stateTransition;
	

end architecture rtl;
