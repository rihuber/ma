use std.textio.all;

library ieee;
use ieee.std_logic_textio.all;
use ieee.std_logic_1164.all;

package simulationPkg is
	
	-- the global clock
	signal clk	: std_logic := '0';
   
	-- signal to broadcast the end of simulation
	signal endOfSimulation : boolean := false;
	
	-- clock generator
	procedure clkGenerator(
		signal clk				: out std_logic;
		constant clkphaselow	: in  time;
		constant clkphasehigh	: in  time
	);
	
	-- opens stimuli and response files
	procedure initPipes(stimuliFilename: in string; responseFileName: in string);
	attribute foreign of initPipes : procedure is "initPipes simulationPkg/fileiopatch.so";
	
	-- reads a string of (stimulus'lenth) characters form the stimuli file
	procedure readNextStimulusString(stimulus: out string; errorCode: out integer);
	attribute foreign of readNextStimulusString : procedure is "readNextStimulusString simulationPkg/fileiopatch.so";
	
	-- returns 'false' if the stimuli file has reached EOF, 'true' otherwise 
	function moreStimuliAvailable return boolean;
	
	-- helper function for 'moreStimuliAvailable'
	-- needed because nli does only support integer data types
	function checkEndOfStimuliFile return integer;
	attribute foreign of checkEndOfStimuliFile : function is "checkEndOfStimuliFile simulationPkg/fileiopatch.so";
	
	-- read 1 character from stimuli file and cast it to a std_logic variable
	procedure readValue(value: out std_logic);
	
	procedure writeValue(outLine: inout line; value: in std_logic);
	
	-- close stimuli file
	procedure closeStimuliFile;
	attribute foreign of closeStimuliFile : procedure is "closeStimuliFile simulationPkg/fileiopatch.so";
	
end package simulationPkg;


	
package body simulationPkg is

	-- generate a periodic but stoppable clock signal
	procedure clkGenerator(
		signal clk				: out std_logic;
		constant clkphaselow	: in  time;
		constant clkphasehigh	: in  time
	) is
	begin
		clkGen: loop
			clk <= '1';
			wait for clkphasehigh;
			clk <= '0';
			wait for clkphaselow;
         	exit clkGen when endOfSimulation = true;
		end loop clkGen;
	wait; -- forever
	end procedure clkGenerator;
	
	procedure readValue(value: out std_logic) is
		variable stringValue: string(1 to 1);
		variable errorCode: integer;
	begin
		readNextStimulusString(stringValue, errorCode);
		if errorCode <= 0 then
			report "Error while fetching std_logic value. Error code: " & integer'image(errorCode) severity ERROR;
		end if;
		value := '0';
		if stringValue = "1" then
			value := '1';
		end if; 
	end procedure readValue;
	
	procedure writeValue(outLine: inout line; value: in std_logic) is
	begin
		write(outLine, value);
		write(outLine, string'(" "));
	end procedure writeValue;
	
	function moreStimuliAvailable return boolean is
	begin
		if checkEndOfStimuliFile > 0 then
			return true;
		end if;
		return false;
	end function moreStimuliAvailable;
	
	function checkEndOfStimuliFile return integer is
	begin
		report "Foreign subprogram checkEndOfStimuliFile not called" severity error;
		return 0;
	end;
	
	procedure initPipes(stimuliFilename: in string; responseFileName: in string) is
	begin
		report "Foreign subprogram initPipes not called" severity error;
	end;
	
	procedure readNextStimulusString(stimulus: out string; errorCode: out integer) is
	begin
		report "Foreign subprogram readNextStimulusString not called" severity error;
	end;
	
	procedure closeStimuliFile is
	begin
		report "Foreign subprogram closeStimuliFile not called" severity error;
	end;
	
end package body simulationPkg;
