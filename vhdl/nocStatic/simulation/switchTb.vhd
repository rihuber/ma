use std.textio.all;

library ieee;
use ieee.std_logic_1164.all;

use work.simulationPkg.all;

use work.switchPkg.all;
use work.headerPkg.all;

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
		reset			: std_logic;
		inputLinksIn	: inputLinkInArray(numPorts-1 downto 0);
		outputLinksIn	: outputLinkInArray(numPorts-1 downto 0);
	end record mutInputType;
	
	-- Output type
	type mutOutputType is record
		inputLinksOut	: inputlinkOutArray(numPorts-1 downto 0);
		outputLinksOut	: outputLinkOutArray(numPorts-1 downto 0);
	end record MutOutputType;
	
	-- Signals for input and output
	signal mutInput 	: mutInputType;
	signal mutOutput	: mutOutputType;
	
	-- Stimuli and response files
	constant stimuli_filename : string := "namedPipes/stimuli";
	constant response_filename : string := "namedPipes/response";
	
	procedure setNewStimulus(stimulus: out mutInputType; errorCode: out integer) is
	begin
		readValue(stimulus.reset, errorCode);
		for i in 0 to numPorts-1 loop
			readValue(stimulus.inputLinksIn(i).empty, errorCode);
			readValue(stimulus.inputLinksIn(i).data, errorCode);
		end loop;
		for i in 0 to numPorts-1 loop
			readValue(stimulus.outputLinksIn(i).full, errorCode);
		end loop;
	end procedure setNewStimulus;
	
	procedure writeResponse(response: in mutOutputType) is
		variable out_line : line;
	begin
		for i in 0 to numPorts-1 loop
			writeValue(response.inputLinksOut(i).readEnable);
		end loop;
		for i in 0 to numPorts-1 loop
			writeValue(response.outputLinksOut(i).writeEnable);
			writeValue(response.outputLinksOut(i).data);
		end loop;
		writeEndOfLine;
	end procedure writeResponse;
	
	constant mutGlobalAddress: globalAddr := "0001";
	
begin

	-- The MUT
	mut: entity work.switch
		generic map(
			globalAddress => mutGlobalAddress
		)
		port map (
			clk				=> clk,
			reset			=> mutInput.reset,
			inputLinksIn	=> mutInput.inputLinksIn,
			inputLinksOut	=> mutOutput.inputLinksOut,
			outputLinksIn	=> mutInput.outputLinksIn,
			outputLinksOut	=> mutOutput.outputLinksOut
		);

	-- Stimuli application
	stimuliApplication: process
		variable mutInputVar: mutInputType;
		variable errorCode: integer;
	begin
		mutInput.reset <= '0';
		initPipes(stimuli_filename, response_filename);
		while moreStimuliAvailable loop
			wait until clk'event and clk = '1';
			wait for stimuli_application_time;
			setNewStimulus(mutInputVar, errorCode);
			if errorCode > 0 then
				mutInput <= mutInputVar;
			end if;
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
			writeResponse(mutOutput);
		end loop;
		closeResponseFile;
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
