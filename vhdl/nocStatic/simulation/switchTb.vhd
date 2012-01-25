use std.textio.all;

library ieee;
use ieee.std_logic_1164.all;


use work.simulationPkg.all;

entity switchTb is
  -- a testbench does not connect to any higher level of hierarchy
end switchTb;


architecture behavioral of switchTb is
	
	-- Timing constants
	constant clk_phase_high            : time := 50 ns;
	constant clk_phase_low             : time := 50 ns;
	constant response_acquisition_time : time := 90 ns;
	constant stimuli_application_time  : time := 10 ns;
	
	-- Input type
	type mutInputType is record
		reset	: std_logic;
		data	: std_logic;
	end record mutInputType;
	
	-- Output type
	type mutOutputType is record
		data	: std_logic;
	end record MutOutputType;
	
	-- Signals for input and output
	signal mutInput 	: mutInputType;
	signal mutOutput	: mutOutputType;
	
	-- Stimuli and response files
	constant stimuli_filename : string := "namedPipes/stimuli";
	constant response_filename : string := "namedPipes/response";
	file responseFile : text open write_mode is response_filename;
	
	procedure setNewStimulus(stimulus: out mutInputType) is
	begin
		readValue(stimulus.reset);
		readValue(stimulus.data);
	end procedure setNewStimulus;
	
	procedure writeResponse(response: in mutOutputType; file responseFile: text) is
		variable out_line : line;
	begin
		writeValue(out_line, response.data);
		writeline(responseFile, out_line);
	end procedure writeResponse;
	
begin

	-- The MUT
	mut: entity work.dummy
	    port map (
	      clk		=> clk,
	      reset		=> mutInput.reset,
	      input		=> mutInput.data,
	      output	=> mutOutput.data
		);

	-- Stimuli application
	stimuliApplication: process
		variable mutInputVar: mutInputType;
	begin
		initPipes(stimuli_filename, response_filename);
		while moreStimuliAvailable loop
			wait until clk'event and clk = '1';
			wait for stimuli_application_time;
			setNewStimulus(mutInputVar);
			mutInput <= mutInputVar;
		end loop;
		endOfSimulation <= true;
		closeStimuliFile;
		wait; -- forever
	end process stimuliApplication;
  
	-- Response acquisition
	responseAcquisition: process
    begin
		while endOfSimulation = false loop
			wait until clk'event and clk = '1';
			wait for response_acquisition_time;
			writeResponse(mutOutput, responseFile);
		end loop;
		file_close(responseFile);
		report "Simulation run completed!";
		wait; -- forever
	end process responseAcquisition;
	
	-- Clock generator
	clkGen: clkGenerator
		(
			clk				=> clk,	       
			clkphaselow		=> clk_phase_low,     
			clkphasehigh	=> clk_phase_high 
		);

end architecture behavioral;
