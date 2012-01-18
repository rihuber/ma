library ieee;
use ieee.std_logic_1164.all;

use work.routerPkg.all;

entity rxPort is
	port (
		clk	 		: in std_logic;
		reset 		: in std_logic;
		
		txPortNrIn	: in portNr;
		txPortNrOut	: out portNrWrapper;
		
		writeEnable	: in std_logic;
		endOfPacket	: in std_logic
	);
end entity rxPort;

architecture rtl of rxPort is
	
	signal txPortNr_p, txPortNr_n : portNrWrapper;
	
begin

	txPortNrOut <= txPortNr_p;
	
	nomem_nextState : process(txPortNr_p, writeEnable, txPortNrIn, endOfPacket) is
	begin
		-- default assignments
		txPortNr_n <= txPortNr_p;
		
		if writeEnable = '1' then
			txPortNr_n <= toPortNrWrapper(txPortNrIn);
		end if;
		if endOfPacket='1' then
			txPortNr_n <= portNrUndefined;
		end if;
	end process nomem_nextState;
	
	mem_stateTransition : process(clk, reset) is
	begin
		if reset = '0' then
			txPortNr_p <= portNrUndefined;
		elsif rising_edge(clk) then
			txPortNr_p <= txPortNr_n;
		end if;
	end process mem_stateTransition;
	

end architecture rtl;
