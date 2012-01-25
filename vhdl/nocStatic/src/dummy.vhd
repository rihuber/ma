library ieee;
use ieee.std_logic_1164.all;

entity dummy is
	port (
		clk : in std_logic;
		reset : in std_logic;
		
		input : in std_logic;
		output: out std_logic
	);
end entity dummy;

architecture RTL of dummy is
	
begin

	output <= input;

end architecture RTL;
