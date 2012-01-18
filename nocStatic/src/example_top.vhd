library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity top is
  port (
  	ledIO : out std_logic_vector(4 downto 0);
    sys_clk_p : in std_logic;
    sys_clk_n : in std_logic
  );
end top;



architecture rtl of top is
	
	signal counter_p, counter_n : unsigned(27 downto 0);
	signal output_p, output_n: std_logic;
	
	signal clk200 : std_logic;
	
begin

	ibufgds_8 : IBUFGDS
    	port map (
	      I => sys_clk_p,
	      IB => sys_clk_n,
	      O => clk200
    	);
    	
    	
	p_mem : process (clk200) is
	begin
		if rising_edge(clk200) then
			counter_p <= counter_n;
			output_p <= output_n;
		end if;
	end process p_mem;
	
		
	p_nomem_counter : process (counter_p) is
	begin
		if counter_p >= 200000000 then
			counter_n <= (others => '0');
		else
			counter_n <= counter_p+1;
		end if;
	end process p_nomem_counter;
	
	p_nomem_output : process (counter_p, output_p) is
	begin
		output_n <= output_p;
		if counter_p = 200000000 then
			output_n <= not output_p;
		end if;
	end process p_nomem_output;
	

	ledIO(0) <= output_p;
	ledIO(4 downto 1) <= (others => '0');
	
	
end architecture rtl;