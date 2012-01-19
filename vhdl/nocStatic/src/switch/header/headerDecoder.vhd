library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.headerPkg.all;

entity headerDecoder is
		port (
		data		: in std_logic_vector(dataWidth-1 downto 0);
		
		destAddr	: out address;
		prio		: out priority
	);
end entity headerDecoder;

architecture rtl of headerDecoder is
	
begin

	destAddr <= extractAddress(data);
	prio <= extractPrio(data);
	
end architecture rtl;
