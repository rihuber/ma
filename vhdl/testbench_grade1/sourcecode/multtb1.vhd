-- Mission: Provide a code example for a file-based testbench.
-- Functionality: A simple testbench for a 4-bit shift add multiplier. The
--    source code of the multiplier is located in the file shiftaddmult.vhd.
--    The testbench reads stimuli and expected responses from separate
--    ASCII files and writes a simulation report to a third ASCII file.
--    The testbench applies stimuli and acquires actual responses every clock
--    cycle.
--    When stimuli vectors outnumber expected responses, the simulation run
--    continues with "don't cares".
-- Companion files: simulstuff.vhd, multtb1pkg.vhd, shiftaddmult.vhd,
--    multref.vhd
-- Platform: This testbench was written and tested with Modelsim 6.0.
-- Company: Microelectronics Design Center, ETH Zurich.
-- Authors: Hubert Kaeslin, Thomas Kuch
-------------------------------------------------------------------------------

use std.textio.all;
library ieee;
use ieee.std_logic_textio.all;  -- read and write overloaded for std_logic
use ieee.std_logic_1164.all;
use work.simulstuff.all;
use work.multTb1Pkg.all;

-------------------------------------------------------------------------------

entity MultTb1 is
  -- a testbench does not connect to any higher level of hierarchy
end MultTb1;

-------------------------------------------------------------------------------

architecture Behavioral of MultTb1 is

  -- declaration of model under test (MUT) and functional
  -- reference (expected response pickup)
  component ShiftAddMult is
    generic (
      width          : natural );  
    port (
      ClkxCI         : in  std_logic;
      RstxRBI        : in  std_logic;
      StartCalcxSI   : in  std_logic;
      InputAxDI      : in  std_logic_vector(width-1 downto 0);
      InputBxDI      : in  std_logic_vector(width-1 downto 0);
      OutputxDO      : out std_logic;
      OutputValidxSO : out std_logic );
  end component;

  for RefInst : ShiftAddMult use entity work.ShiftAddMult(Pickup);
  for MutInst : ShiftAddMult use entity work.ShiftAddMult(Behavioral);

begin

  -- instantiate MUT and functional reference and connect them to the
  -- testbench signals
  -- note: any bidirectional must connect to both stimuli and responses
  -----------------------------------------------------------------------------

  MutInst : ShiftAddMult
    generic map (
      width          => 4 )
    port map (
      ClkxCI         => ClkxC,
      RstxRBI        => StimuliRecxD.RstxRB,
      StartCalcxSI   => StimuliRecxD.StartCalcxS,
      InputAxDI      => StimuliRecxD.InputAxD,
      InputBxDI      => StimuliRecxD.InputBxD,
      OutputxDO      => ActResponseRecxD.OutputxD,
      OutputValidxSO => ActResponseRecxD.OutputValidxS );

  RefInst : ShiftAddMult
    generic map (
      width          => 4 )
    port map (
      ClkxCI         => ClkxC,
      RstxRBI        => StimuliRecxD.RstxRB,
      StartCalcxSI   => StimuliRecxD.StartCalcxS,
      InputAxDI      => StimuliRecxD.InputAxD,
      InputBxDI      => StimuliRecxD.InputBxD,
      OutputxDO      => ExpResponseRecxD.OutputxD,
      OutputValidxSO => ExpResponseRecxD.OutputValidxS );

  
  -- pausable clock generator with programmable mark and space widths
  -----------------------------------------------------------------------------
  -- The procedure ClockGenerator is defined in the package simulstuff.
  -- This concurrent procedure call is a process that calls the procedure,
  -- with a syntax that looks like a "process instance".
  
  ClkGen : ClockGenerator(
     ClkxC        => ClkxC,	       
     clkphaselow  => clk_phase_low,     
     clkphasehigh => clk_phase_high );
  
  
  -- obtain stimuli and apply it to MUT
  -----------------------------------------------------------------------------
  StimAppli : process
  begin
    
    AppliLoop : while not (endfile(stimulifile)) loop
      wait until ClkxC'event and ClkxC = '1';
      
      -- wait until time has come for stimulus application
      wait for stimuli_application_time;
      -- apply stimulus to MUT
      StimuliRecxD <= GetStimuliRecord(stimulifile);      
    end loop AppliLoop;
    
    -- tell clock generator to stop at the end of current cycle
    -- because stimuli have been exhausted
    EndOfSimxS <= true;
    -- close the file
    file_close(stimulifile);
    wait;
  end process StimAppli;
  

  -- acquire actual response from MUT and have it checked
  -----------------------------------------------------------------------------
  RespAcqui : process
    
    -- variables for accounting of mismatching responses
    variable respmatch : respMatchArray;
    variable respaccount : respaccounttype := (0, 0, 0, 0, 0, 0);
    -- variable for counting the lines written to the simulation report
    variable simRepLineCount : natural := 0;

  begin
    -- This wait statement is only useful if the stimuli file is empty. In that 
    -- case, EndOfSimxS gets true after one delta delay. Without the wait, the 
    -- exit statement below would be executed before EndOfSimxS gets true.
    wait until ClkxC'event and ClkxC = '0'; 

    AcquiLoop : loop
      -- leave the loop if there are no more stimuli left
      exit AcquiLoop when EndOfSimxS = true;
      
      wait until ClkxC'event and ClkxC = '1';

      -- wait until time has come for response acquisition
      wait for response_acquisition_time;
          
      -- compare the actual with the expected responses
      CheckResponse(ActResponseRecxD, ExpResponseRecxD,
                     respmatch, respaccount);

      -- add a trace line to report file
      PutSimulationReportTrace(simreptfile, StimuliRecxD, ActResponseRecxD,
                               respmatch, respaccount, simRepLineCount);

      -- add extra failure message to report file if necessary
      PutSimulationReportFailure(simreptfile, ExpResponseRecxD, respmatch);

    end loop AcquiLoop;

    -- when the present clock cycle is the final one of this run
    -- then establish a simulation report summary and write it to file
    PutSimulationReportSummary(simreptfile, respaccount);
    -- close the file
    file_close(simreptfile);
    report "Simulation run completed!";
    wait;    
  end process RespAcqui;

end architecture Behavioral;  -- of MultTb1
