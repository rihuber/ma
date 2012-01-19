-- Mission: See associated testbench file.
-- Functionality: These Package contains type declarations, signals and
--   constants for the testbench. It also contains some MUT-specific functions
--   and procedures. 
-- Platform: Modelsim 6.0.
-- Company: Microelectronics Design Center, ETH Zurich.
-- Authors: Hubert Kaeslin, Thomas Kuch
-- Version: 1.1  10.11.2008, kgf added 'impure' 
-------------------------------------------------------------------------------

use std.textio.all;
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use work.simulstuff.all;

--=============================================================================

package multTb1Pkg is

  -- declarations of all those signals that do connect to the MUT
  -- most of them are collected in records to facilitate data handling
  -- note: any bidirectional must be made part of both stimuli and responses
  type stimuliRecordType is record
    RstxRB      : std_logic;
    StartCalcxS : std_logic;
    InputAxD    : std_logic_vector(3 downto 0);
    InputBxD    : std_logic_vector(3 downto 0);
  end record;
  
  -- same for actual and expected response
  type responseRecordType is record
    OutputxD      : std_logic;
    OutputValidxS : std_logic;
  end record;

  -- as there are two elements in the response record, an array with two
  -- elements of type respmatchtype is needed 
  type respMatchArray is array (1 to 2) of respmatchtype;
  
  signal ClkxC            : std_logic := '1';    -- driving clock
  signal StimuliRecxD     : stimuliRecordType;   -- record of stimuli
  signal ActResponseRecxD : responseRecordType;  -- record of actual responses
  signal ExpResponseRecxD : responseRecordType;  -- record of expected responses

  -- timing of clock and simulation events
  constant clk_phase_high            : time := 50 ns;
  constant clk_phase_low             : time := 50 ns;
  constant response_acquisition_time : time := 90 ns;
  constant stimuli_application_time  : time := 10 ns;

  -- declaration of stimuli, expected responses, and simulation report files
  constant stimuli_filename : string := "../simvectors/stimuli.asc";
  constant expresp_filename : string := "../simvectors/expresp.asc";
  constant simrept_filename : string := "../simvectors/simrept.asc";

  -- the files are opened implicitly right here
  file stimulifile : text open read_mode  is stimuli_filename;
  file simreptfile : text open write_mode is simrept_filename;

-------------------------------------------------------------------------------

  -- function for reading stimuli data from the stimuli file
  impure function GetStimuliRecord (file stimulifile : text)
    return stimuliRecordType;

  -- function for reading expected responses from the expected response file
  impure function GetExpectedResponseRecord (file exprespfile : text)
    return responseRecordType;

  -- procedure for comparing actual and expected response
  procedure CheckResponse
    (actRespRecord : in    responseRecordType;
     expRespRecord : in    responseRecordType;
     respmatch     : inout respMatchArray;
     respaccount   : inout respaccounttype);
  
  -- procedure for writing stimuli and actual responses to the report file                     
  procedure PutSimulationReportTrace
    (file simreptfile :    text;
     stimuliRecord   : in stimuliRecordType;
     actRespRecord   : in responseRecordType;
     respmatch       : in respMatchArray;
     respaccount     : in respaccounttype;
     simRepLineCount : inout natural);
  
  -- compose a failure message line and write it to the report file
  procedure PutSimulationReportFailure
    (file simreptfile :    text;
     expRespRecord    : in responseRecordType;
     respmatch        : in respMatchArray);

end package MultTb1Pkg;

--=============================================================================

package body MultTb1Pkg is
  
    -- purpose: get one record worth of stimuli from file.
  impure function GetStimuliRecord
    (file stimulifile : text)
    return stimuliRecordType
  is
    variable in_line, in_line_tmp : line;
    -- stimuli to default to unknown in case no value is obtained from file
    variable stimulirecord : stimuliRecordType := 
      (RstxRB => 'X', StartCalcxS => 'X', 
      InputAxD => (others => 'X'), InputBxD => (others => 'X') );
  begin
    -- read a line from the stimuli file
    -- skipping any empty and comment lines encountered
    loop
      readline(stimulifile, in_line);
      -- copy line read to enable meaningful error messages later
      in_line_tmp := new string'(in_line(in_line'low to in_line'high));
      if in_line_tmp'length >= 1 then
        exit when in_line_tmp(1) /= '%';
      end if;
      deallocate(in_line_tmp);
    end loop;
    -- extract all values of a record of stimuli
    GetFileEntry(stimulirecord.RstxRB, in_line, in_line_tmp, stimuli_filename);
    GetFileEntry(stimulirecord.StartCalcxS, in_line, in_line_tmp, 
      stimuli_filename);
    GetFileEntry(stimulirecord.InputAxD, in_line, in_line_tmp, 
      stimuli_filename);
    GetFileEntry(stimulirecord.InputBxD, in_line, in_line_tmp, 
      stimuli_filename);
    -- deallocate line copy now that all entries have been read
    deallocate(in_line_tmp);
    return stimulirecord;
  end GetStimuliRecord;

-------------------------------------------------------------------------------

  -- purpose: get one record worth of expected responses from file.
  impure function GetExpectedResponseRecord (file exprespfile : text)
    return responseRecordType
  is
    variable in_line, in_line_tmp : line;
    -- expected responses to default to don't care
    -- in case no value is obtained from file
    variable expresprecord : responseRecordType := 
      (OutputxD => '-', OutputValidxS => '-');
  begin
    -- read a line from the expected response file as long as there are any
    -- skipping any empty and comment lines encountered      
    if not(endfile(exprespfile)) then
      loop
        readline(exprespfile, in_line);
        -- copy line read to enable meaningful error messages later
        in_line_tmp := new string'(in_line(in_line'low to in_line'high));
        if in_line_tmp'length >= 1 then
          exit when in_line_tmp(1) /= '%';
        end if;
        deallocate(in_line_tmp);
      end loop;
      -- extract all values of a record of expected responses
      GetFileEntry(expresprecord.OutputxD, in_line, in_line_tmp, 
        expresp_filename);
      GetFileEntry(expresprecord.OutputValidxS, in_line, in_line_tmp, 
        expresp_filename);
      -- deallocate line copy now that all entries have been read
      deallocate(in_line_tmp);
      -- return default value in case EOF is overrun, no else clause needed
    end if;
    return expresprecord;
  end GetExpectedResponseRecord;

-------------------------------------------------------------------------------

  --  purpose: procedure for comparing actual and expected response
  procedure CheckResponse
    (actRespRecord : in    responseRecordType;
     expRespRecord : in    responseRecordType;
     respmatch     : inout respMatchArray;
     respaccount   : inout respaccounttype)
  is
  begin
    
    CheckValue(ActResponseRecxD.OutputxD, ExpResponseRecxD.OutputxD,
                  respmatch(1), respaccount);
    CheckValue(ActResponseRecxD.OutputValidxS, ExpResponseRecxD.OutputValidxS,
                  respmatch(2), respaccount);
    
  end CheckResponse;

-------------------------------------------------------------------------------
  
  -- purpose: writing stimuli and actual responses to the report file.
  procedure PutSimulationReportTrace
    (file simreptfile :    text;
     stimuliRecord    : in stimuliRecordType;
     actRespRecord    : in responseRecordType;
     respmatch        : in respMatchArray;
     respaccount      : in respaccounttype;
     simRepLineCount  : inout natural)
  is
    constant N        : natural := 60;
    variable out_line : line;
  begin
    -- every N-th line, [re]write the signal caption to the simulation report
    if simRepLineCount mod N = 0 then
      write(out_line, 
        string'(" "));
      writeline(simreptfile, out_line);
      write(out_line, 
        string'("Time            RstxRB                  OutputxD"));
      writeline(simreptfile, out_line);
      write(out_line, 
        string'("|               |   StartCalcxSI           |  OutputValidxS"));
      writeline(simreptfile, out_line);
      write(out_line, 
        string'("|               |   |   InputAxD           |     |"));
      writeline(simreptfile, out_line);
      write(out_line, 
        string'("|               |   |   3210   InputBxD    |     |"));
      writeline(simreptfile, out_line);
      write(out_line, 
        string'("|               |   |   |      3210        |     |"));
      writeline(simreptfile, out_line);
      write(out_line, 
        string'("|               |   |   |      |           |     |"));
      writeline(simreptfile, out_line);
    end if;
    simRepLineCount := simRepLineCount + 1;
    
    -- begin with simulation time
    write(out_line, string'("at "));
    write(out_line, now);
    -- add stimuli
    write(out_line, ht);
    write(out_line, stimuliRecord.RstxRB);
    write(out_line, string'("   "));
    write(out_line, stimuliRecord.StartCalcxS);
    write(out_line, string'("   "));
    write(out_line, stimuliRecord.InputAxD);
    write(out_line, string'("   "));
    write(out_line, stimuliRecord.InputBxD);
    -- add actual response 1
    write(out_line, string'("        "));
    write(out_line, actRespRecord.OutputxD);

    case respmatch(1) is
      when mok =>
        -- if the actual response matches with the expected one, append nothing
        write(out_line, string'("     "));                             
      when mne =>
        -- if there was no expected response for the actual one, append a '-' 
        write(out_line, string'(" -   "));                                       
      when mlf =>
        -- if the actual response doesn't match logically, append an 'l'
        write(out_line, string'(" l   "));
      when msf =>
        -- if the actual doesn't match in strength, append an 's'
        write(out_line, string'(" s   "));                       
      when others =>  -- when mil
        -- if the actual response is "don't care", append an 'i'
        write(out_line, string'(" i   "));  -- 
    end case;                   

    -- add actual response 2
    write(out_line, actRespRecord.OutputValidxS);

    case respmatch(2) is
      when mok => null;
      when mne =>
        write(out_line, string'(" -")); 
      when mlf =>
        write(out_line, string'(" l"));
      when msf =>
        write(out_line, string'(" s"));
      when others =>  -- when mil
        write(out_line, string'(" i")); 
    end case;
    
    -- write the output line to the report file
    writeline(simreptfile, out_line);
  end PutSimulationReportTrace;

-------------------------------------------------------------------------------
    
  -- purpose: compose a failure message line and write it to the report file.
  procedure PutSimulationReportFailure
    (file simreptfile :    text;
     expRespRecord    : in responseRecordType;
     respmatch        : in respMatchArray)
  is
    variable out_line : line;
  begin
    
    -- if at least one actual doesn't match with its expected response
    if (respmatch(1) /= mok and respmatch(1) /= mne) or
       (respmatch(2) /= mok and respmatch(2) /= mne) then

      write(out_line, string'("^^ Failure! Expected was :"));

      -- if actual response 1 doesn't match with its expected response
      if respmatch(1) /= mok and respmatch(1) /= mne then
        -- add expected response
        write(out_line, string'("                 "));
        write(out_line, expRespRecord.OutputxD);
        
      else
        write(out_line, string'("                  "));
      end if;

      -- if actual response 2 doesn't match with its expected response
      if respmatch(2) /= mok and respmatch(2) /= mne then
        -- add expected response
        write(out_line, string'("     "));
        write(out_line, expRespRecord.OutputValidxS);
      end if;

      writeline(simreptfile, out_line);
    end if;
  end PutSimulationReportFailure;

end package body MultTb1Pkg;
