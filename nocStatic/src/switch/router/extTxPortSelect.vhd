library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.routerPkg.all;

entity extTxPortSelect is
	port (
		rxPortNrIn 			: in portNr;
		rxPortNrOut			: out portNr;
		
		txPortIdle			: in std_logic_vector(numExtPorts-1 downto 0);
		txPortWriteEnable	: out std_logic_vector(numExtPorts-1 downto 0);
		
		txFifoReadEnable	: out std_logic;
		txFifoEmpty			: in std_logic;
		
		txPortNrOut			: out portNr;
		rxPortWriteEnable	: out std_logic_vector(numPorts-1 downto 0)
	);
end entity extTxPortSelect;

architecture rtl of extTxPortSelect is
	
	function selectTxPort(txPortIdle: std_logic_vector(numExtPorts-1 downto 0)) return portNrWrapper is
		variable result : portNrWrapper;
	begin
		result := 0;
		while result < numExtPorts loop
			if txPortIdle(result) = '0' then
				return result + numIntPorts;
			end if;
			result := result + 1;
		end loop;
		return result + numIntPorts;
	end selectTxPort;
	
begin

	rxPortNrOut <= rxPortNrIn;
	
	nomem_output:process(txPortIdle, txFifoEmpty, rxPortNrIn) is
		variable txPortNr : portNrWrapper;
	begin
		-- default assignments
		txPortWriteEnable <= (others => '0');
		txFifoReadEnable <= '0';
		rxPortWriteEnable <= (others => '0');
		txPortNrOut <= (others => '-');
		
		if txFifoEmpty = '0' then
			txPortNr := selectTxPort(txPortIdle);
			if txPortNr /= portNrUndefined then
				txPortWriteEnable(txPortNr) <= '1';
				txFifoReadEnable <= '1';
				txPortNrOut <= txPortNr;
				rxPortWriteEnable(rxPortNrIn) <= '1';
			end if;
		end if;
			
	end process nomem_output;

end architecture rtl;
