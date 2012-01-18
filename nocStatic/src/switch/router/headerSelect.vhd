library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.headerPkg.all;
use work.utilPkg.all;
use work.routerPkg.all;

entity headerSelect is
	port (
		headerIn		: in headerArray(numPorts-1 downto 0);
		selected		: out std_logic_vector(numPorts-1 downto 0);
		
		dataValid		: out std_logic;
		selectedRxPort	: out portNr;
		selectedAddr	: out address
	);
end entity headerSelect;


architecture rtl of headerSelect is

	function selectHeader(headers:headerArray(numPorts-1 downto 0)) return portNrWrapper is
		variable rxPortNr : portNrWrapper;
	begin
		for pri in numPriorities-1 downto 0 loop
			rxPortNr := selectHeaderWithPrio(headers,pri);
			if rxPortNr /= portNrUndefined then
				return rxPortNr;
			end if;
		end loop;
		return portNrUndefined;
	end selectHeader;	
	
	function selectHeaderWithPrio(headers:headerArray(numPorts-1 downto 0); searchedPrio:priority) return portNrWrapper is
		variable rxPortNr : portNrWrapper;
	begin
		rxPortNr := 0;
		while rxPortNr<numPorts-1 loop
			if headers(rxPortNr).valid='1' then
				if headers(rxPortNr).prio = searchedPrio then
					return rxPortNr;
				end if;
			end if;
		end loop;
		return portNrUndefined;
	end selectHeaderWithPrio;

begin
	
	nomem_output : process(headerIn)
		variable rxPortNr : portNrWrapper;
	begin
		-- default assignments
		selected <= (others => '0');
		dataValid <= '0';
		selectedRxPort <= (others => '-');
		selectedAddr <= (others => '-');
		
		rxPortNr := selectHeader(headerIn);
		if rxPortNr /= portNrUndefined then
			selected(rxPortNr) <= '1';
			dataValid <= '1';
			selectedRxPort <= toPortNr(rxPortNr);
			selectedAddr <= headerIn(rxPortNr).addr;
		end if;
		
	end process nomem_output;
end architecture rtl;
