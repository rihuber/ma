library IEEE;
use ieee.std_logic_1164.all;

use work.utilPkg.all;

package headerPkg is
	
	constant dataWidth		: integer := 8;
	
	constant addressWidth 	: integer := 6;
	constant localAddrWidth	: integer := 2;
	constant globalAddrWidth: integer := addressWidth - localAddrWidth;
	
	constant priorityWidth 	: integer := dataWidth - addressWidth;
	constant numPriorities	: integer := toPow2(priorityWidth);
	
	subtype localAddr is std_logic_vector(localAddrWidth-1 downto 0);
	subtype globalAddr is std_logic_vector(globalAddrWidth-1 downto 0);
	type address is record
		local	: localAddr;
		global	: globalAddr;
	end record;
	
	subtype priority is std_logic_vector(priorityWidth-1 downto 0);
	
	type header is record
		valid	: std_logic;
		addr	: address;
		prio	: priority;
	end record;
	
	type headerArray is array(natural range <>) of header;
	
	function extractAddress(headerBits: std_logic_vector(dataWidth-1 downto 0)) return address;
	function extractPrio(headerBits: std_logic_vector(dataWidth-1 downto 0)) return priority;
		
end package headerPkg;

package body headerPkg is
	
	function extractAddress(headerBits: std_logic_vector(dataWidth-1 downto 0)) return address is
		variable result: address;
	begin
		result.global := headerBits(globalAddrWidth-1 downto 0);
		result.local := headerBits(addressWidth-1 downto globalAddrWidth);
		return result;
	end extractAddress;
	
	function extractPrio(headerBits: std_logic_vector(dataWidth-1 downto 0)) return priority is
		variable result: priority;
	begin
		result := headerBits(dataWidth-1 downto addressWidth);
	end extractPrio;
	
end package body headerPkg;
