library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.headerPkg.all;

entity headerDecoder is
		port (
		data		: in std_logic_vector(7 downto 0);
		
		destAddr	: out std_logic_vector(addressWidth-1 downto 0);
		priority	: out std_logic_vector(priorityWidth-1 downto 0)
	);
end entity headerDecoder;

architecture rtl of headerDecoder is
	
begin

	destAddr <= extractAddress(data);
	priority <= extractPrio(data);
	
end architecture rtl;
