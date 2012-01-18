library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.utilPkg.all;
use work.headerPkg.all;
use work.switchPkg.all;

entity txFifoSelect is
	generic(
		globalAddress	: globalAddr
	);
	port (
		selectedAddr	: in address;
		dataValid		: in std_logic;
		
		fifoWriteEnable	: out std_logic_vector(numIntPorts downto 0) -- one for each internal fifo and one for the global fifo
	);
end entity txFifoSelect;

architecture rtl of txFifoSelect is
	constant externalFifoIndex : unsigned(numIntPorts downto 0) := to_unsigned(numIntPorts, toLog2Ceil(numIntPorts));
begin

	nomem_output : process (selectedAddr) is
	begin
		-- default assignment
		fifoWriteEnable <= (others => '0');
		
		if dataValid = '1' then
			if selectedAddr.global = globalAddress then
				fifoWriteEnable(selectedAddr.local) <= '1';
			else
				fifoWriteEnable(externalFifoIndex) <= '1';
			end if;
		end if;
	end process nomem_output;

end architecture rtl;
