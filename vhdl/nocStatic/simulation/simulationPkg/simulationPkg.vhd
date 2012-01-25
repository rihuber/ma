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
	
	-- init stimuli and response file names
	procedure initPipes(stimuliFilename: in string; responseFileName: in string);
	
	-- init stimuli file name
	procedure initStimuli(stimuliFilename: in string);
	attribute foreign of initStimuli : procedure is "initStimuli simulationPkg/stimuli.so";
	
	-- init response file name
	procedure initResponse(responseFilename: in string);
	attribute foreign of initResponse : procedure is "initResponse simulationPkg/response.so";
	
	-- reads a string of (stimulus'lenth) characters form the stimuli file
	procedure readNextString(stimulus: out string; errorCode: out integer);
	attribute foreign of readNextString : procedure is "readNextTestString simulationPkg/stimuli.so";
	
	-- writes a string of (response'length) characters to the response file
	procedure writeString(response: in string; errorCode: out integer);
	attribute foreign of writeString : procedure is "writeString simulationPkg/response.so";
	
	-- returns 'false' if the stimuli file has reached EOF, 'true' otherwise 
	function moreStimuliAvailable return boolean;
	
	-- helper function for 'moreStimuliAvailable'
	-- needed because nli does only support integer data types
	function checkEndOfStimuliFile return integer;
	attribute foreign of checkEndOfStimuliFile : function is "checkEndOfFile simulationPkg/stimuli.so";
	
	-- read 1 character from stimuli file and cast it to a std_logic variable
	procedure readValue(value: out std_logic; errorCode: out integer);
	
	procedure writeValue(value: in std_logic);
	
	procedure writeEndOfLine;
	attribute foreign of writeEndOfLine : procedure is "writeEndOfLine simulationPkg/response.so";
	
	-- close stimuli file
	procedure closeStimuliFile;
	attribute foreign of closeStimuliFile : procedure is "closeStimuli simulationPkg/stimuli.so";
	
	-- close response file
	procedure closeResponseFile;
	attribute foreign of closeResponseFile : procedure is "closeResponse simulationPkg/response.so";
	
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
	
	procedure initPipes(stimuliFilename: in string; responseFileName: in string) is
	begin
		initStimuli(stimuliFilename);
		initResponse(responseFilename);
	end procedure initPipes;
	
	procedure readValue(value: out std_logic; errorCode: out integer) is
		variable stringValue: string(1 to 1);
	begin
		readNextString(stringValue, errorCode);
		value := '0';
		if stringValue = "1" then
			value := '1';
		end if; 
	end procedure readValue;
	
	procedure writeValue(value: in std_logic) is
		variable outString: string(1 to 2);
		variable errorCode: integer;
	begin
		if value = '0' then
			outString(1) := '0';
		else
			outString(1) := '1';
		end if;
		outString(2) := ' '; 
		writeString(outString, errorCode);
	end procedure writeValue;
	
	function moreStimuliAvailable return boolean is
	begin
		if checkEndOfStimuliFile = 0 then
			return true;
		end if;
		return false;
	end function moreStimuliAvailable;
	
	function checkEndOfStimuliFile return integer is
	begin
		report "Foreign subprogram checkEndOfStimuliFile not called" severity error;
		return 0;
	end;
	
	procedure initStimuli(stimuliFilename: in string) is
	begin
		report "Foreign subprogram initStimuli not called" severity error;
	end;
	
	procedure initResponse(responseFilename: in string) is
	begin
		report "Foreign subprogram initResponse not called" severity error;
	end;
	
	procedure readNextString(stimulus: out string; errorCode: out integer) is
	begin
		report "Foreign subprogram readNextStimulusString not called" severity error;
	end;
	
	procedure closeStimuliFile is
	begin
		report "Foreign subprogram closeStimuliFile not called" severity error;
	end;
	
	procedure closeResponseFile is
	begin
		report "Foreign subprogram closeResponseFile not called" severity error;
	end;
	
	procedure writeString(response: in string; errorCode: out integer) is
	begin
		report "Foreign subprogram writeString not called" severity error;
	end;
	
	procedure writeEndOfLine is
	begin
		report "Foreign subprogram writeEndOfLine not called" severity error;
	end;
	
end package body simulationPkg;