library ieee;
use ieee.numeric_std.all;

use work.utilPkg.all;

package routerPkg is
	
	constant numPorts 		: integer := 6;
	constant numIntPorts	: integer := 2;
	constant numExtPorts	: integer := numPorts - numIntPorts;
					 
	subtype portNr is unsigned(toLog2Ceil(numPorts-1) downto 0);
	type portNrArray is array(natural range<>) of portNr;
	subtype portNrWrapper is unsigned(toLog2Ceil(numPorts) downto 0);
	type portNrWrapperArray is array(natural range<>) of portNrWrapper;
	
	constant portNrUndefined	: portNrWrapper := to_unsigned(numPorts, toLog2Ceil(numPorts));
	
	function toPortNr(wrappedPortNr: portNrWrapper) return portNr;
	function toPortNrWrapper(unwrappedPortNr: portNr) return portNrWrapper;
	
end package routerPkg;

package body routerPkg is
	
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
	
end package body routerPkg;
