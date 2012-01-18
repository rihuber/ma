library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.utilPkg.all;
use work.headerPkg.all;

package switchPkg is
	
	constant numPorts 		: integer := 6;
	constant numIntPorts	: integer := 2;
	constant numExtPorts	: integer := numPorts - numIntPorts;
	
	
	type inputLinkIn is record
		empty	: std_logic;
		data	: std_logic_vector(dataWidth downto 0);
	end record;
	type inputLinkInArray is array(natural range<>) of inputLinkIn;
	
	type inputLinkOut is record
		readEnable	: std_logic;
	end record;
	type inputLinkOutArray is array(natural range<>) of inputLinkOut;
	
	type outputLinkIn is record
		full	: std_logic;
	end record;
	type outputLinkInArray is array(natural range<>) of outputLinkIn;
	
	type outputLinkOut is record
		writeEnable	: std_logic;
		data		: std_logic_vector(dataWidth downto 0);
	end record;
	type outputLinkOutArray is array(natural range<>) of outputLinkOut;
					 
	subtype portNr is unsigned(toLog2Ceil(numPorts-1) downto 0);
	type portNrArray is array(natural range<>) of portNr;
	subtype portNrWrapper is unsigned(toLog2Ceil(numPorts) downto 0);
	type portNrWrapperArray is array(natural range<>) of portNrWrapper;
	
	constant portNrUndefined	: portNrWrapper := to_unsigned(numPorts, toLog2Ceil(numPorts));
	
	function toPortNr(wrappedPortNr: portNrWrapper) return portNr;
	function toPortNrWrapper(unwrappedPortNr: portNr) return portNrWrapper;
	
end package switchPkg;

package body switchPkg is
	
	function toPortNr(wrappedPortNr: portNrWrapper) return portNr is
	begin
		return wrappedPortNr(toLog2Ceil(numPorts-1) downto 0);
	end toPortNr;
	
	function toPortNrWrapper(unwrappedPortNr: portNr) return portNrWrapper is
		variable result : portNrWrapper;
	begin
		result := (others => '0');
		result(toLog2Ceil(numPorts-1) downto 0) := portNr;
		return result;
	end toPortNrWrapper;
	
end package body switchPkg;
