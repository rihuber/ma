-------------------------------------------------------------------------------
-- File:         simulstuff.vhd
-- Authors:      H.Kaeslin, M.Stadler, P.Luethi, F.Camarero, Th.Kuch, M.Braendli
-- Copyright:	 1995 - 2007 Microelectronics Design Center, ETH Zurich
-- Platform:     ModelSim 6.3
-- Last changes: 23.4.2008
-------------------------------------------------------------------------------
-- Mission:     Take care of all those lower-level issues of testbench
--              writing that make this activity slow and boring when starting
--              from scratch.
-- Description: A collection of universal types and subprograms that are
--              helpful for simulation purposes, not intended for synthesis.
--              By universal we mean independent of the model under test (MUT).
-------------------------------------------------------------------------------
-- Modification history:
--  7.7.98: source code established.
-- 16.7.98: new subprograms added for response evaluation and reporting.
--  4.9.98: failure annotation modified.
-- 31.5.00: generator for random vectors added.
--  7.6.00: CheckResponse overloaded for std_logic as suggested by M.Braendli.
-- 22.6.00: extraction of file entries (read) included for brevity of testb.
-- 07.5.03: added GenerateRandomBit where probability can be controlled.
-- 06.6.03: changed GenerateRandomVector for random numbers in open interval.
-- 18.7.05: new global signal "EndOfSimxS".
-- 25.7.05: new entry "mne" in (enumeration)type "respmatchtype".
-- 26.7.05: procedure PutSimulationReportSummary changed.
-- 09.8.05: CheckResponse renamed to CheckValue.
-- 20.9.05: function AnnotateFailureMessage removed.
-- 21.9.05: various changes and extensions by P.Luethi added:
--          - extensions to stimuli/expresp file input:
--            GetFileEntryXXX for signed/unsigned integer to
--            signed/unsigned/std_logic_vector conversion
--          - CheckValue and write() overload methods for signed & unsigned
-- 23.4.08: some general cleanup
-------------------------------------------------------------------------------
use std.textio.all;
library ieee;
use ieee.std_logic_textio.all;  -- read and write overloaded for std_logic
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.Uniform;     -- IEEE 1076.2 real math package
use ieee.math_real.Trunc;
-------------------------------------------------------------------------------

-- package declaration
package simulstuff is
   
   -- signal to broadcast the end of simulation
   signal EndOfSimxS : boolean := false;
   -- clock generator     
   procedure ClockGenerator (
      signal ClkxC          : out std_logic;
      constant clkphaselow  : in  time;
      constant clkphasehigh : in  time);  

   -- support for file handling
   function FileOpenMessage (
      filename : string; status : file_open_status)
      return string;
   function FileReadMessage (
      filename : string; read_ok : boolean; lineread : string)
      return string;

   -- purpose: get one entry from the stimuli or expected responses file
   -- binary character => std_logic
   procedure GetFileEntry (
      fileentry            : inout std_logic;
      in_line, in_line_tmp : inout line; filename : string);
   -- binary string => std_logic_vector
   procedure GetFileEntry (
      fileentry            : inout std_logic_vector;
      in_line, in_line_tmp : inout line; filename : string);
   -- binary string => unsigned
   procedure GetFileEntry (
      val                  : inout unsigned;
      in_line, in_line_tmp : inout line; filename : string);
   -- binary string => signed
   procedure GetFileEntry (
      val                  : inout signed;
      in_line, in_line_tmp : inout line; filename : string);
   -- unsigned integer string => unsigned
   procedure GetFileEntryInt2x (
      val                  : inout unsigned;
      in_line, in_line_tmp : inout line; filename : string);
   -- signed/unsigned integer string => signed
   procedure GetFileEntryInt2x (
      val                  : inout signed;
      in_line, in_line_tmp : inout line; filename : string);
   -- signed/unsigned integer string => (signed =>) std_logic_vector
   procedure GetFileEntryInt2x (
      val                  : inout std_logic_vector;
      in_line, in_line_tmp : inout line; filename : string);
   -- unsigned integer string => (unsigned =>) std_logic_vector
   procedure GetFileEntryUInt2x (
      val                  : inout std_logic_vector;
      in_line, in_line_tmp : inout line; filename : string);

   -- support for evaluating responses from the MUT
   type respmatchtype is (
      mne,   -- not evaluated = expected response has the value "don't care"
      mok,   -- o.k. = both logic value and drive strength do match
      mlf,   -- logic fail = logic value or tristate status does not match
      msf,   -- strength fail = weak instead of strong drive or viceversa
      mil);  -- illegal response = actual response has value "don't care"
   
   type vectorwise_matchtable
      is array (respmatchtype, respmatchtype) of respmatchtype;
   constant check_vectorwise : vectorwise_matchtable := (
      --     ---------------------------------------
      --     | mne  mok  mlf  msf  mil       |     |   
      --     ---------------------------------------
             ( mne, mok, mlf, msf, mil ), -- | mne |
             ( mok, mok, mlf, msf, mil ), -- | mok |
             ( mlf, mlf, mlf, mlf, mil ), -- | mlf |
             ( msf, msf, mlf, msf, mil ), -- | msf |
             ( mil, mil, mil, mil, mil )  -- | mil |
             );   -- symmetric, dimensions may be interchanged
   
   type respaccounttype is record
      numberof_mch,  -- number of responses checked so far
      numberof_mne,  -- see above for this and all other fields  
      numberof_mok,
      numberof_mlf,
      numberof_msf,
      numberof_mil : natural;
   end record;

   -- checks if actual and expected response are equal
   procedure CheckValue (
      actresp, expresp : in    std_logic_vector;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype);
   procedure CheckValue (
      actresp, expresp : in    std_logic;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype);
   procedure CheckValue (
      actresp, expresp : in    unsigned;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype);
   procedure CheckValue (
      actresp, expresp : in    signed;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype);

   -- purpose: summarize simulation report and write it to report file
   procedure PutSimulationReportSummary
      (file simreptfile : text; respaccount : in respaccounttype);

   -- purpose: overload write for signed and unsigned
   procedure write (outline : inout line; val : in unsigned);
   procedure write (outline : inout line; val : in signed);

   -- support for generation of random test patterns
   procedure GenerateRandomVector (
      randvectwidth        : in    natural;
      statevar1, statevar2 : inout integer;
      randvect             : out   std_logic_vector);  -- unconstrained array type
   procedure GenerateRandomBit (
      probability_of_1     : in    real;
      statevar1, statevar2 : inout integer;
      randbit              : out   std_logic);
   procedure GenerateRandomInteger (
      min, max             : in    integer;
      statevar1, statevar2 : inout integer;
      randinteger          : out   integer);

   
end package simulstuff;
--=============================================================================

-- package body
package body simulstuff is
   
   -- purpose: generate a periodic but stoppable clock signal,
   -- generator to be instantiated as a concurrent procedure.
   procedure ClockGenerator
      (signal ClkxC          : out std_logic;
       constant clkphaselow  : in  time;
       constant clkphasehigh : in  time)
   is
   begin
      ClkGen : loop
         ClkxC <= '1';
         wait for clkphasehigh;
         ClkxC <= '0';
         wait for clkphaselow;
         exit ClkGen when EndOfSimxS = true;
      end loop ClkGen;
      wait;                             -- forever
   end procedure ClockGenerator;
-------------------------------------------------------------------------------

   -- purpose: translate file open status into a human-readable text string.
   function FileOpenMessage (filename : string; status : file_open_status)
      return string is
   begin
      case status is
         when open_ok => return "File "& filename &" opened successfully.";
         when status_error => return "File "& filename &" already opened.";
         when name_error => return
            "File "& filename &" does not exist or can not be created.";
         when mode_error => return
            "File "& filename &" can not be opened in write or append mode.";
      end case;
   end FileOpenMessage;

   -- purpose: translate file read status into a human-readable text string.
   function FileReadMessage (filename : string; read_ok : boolean; lineread : string)
      return string is
   begin
      if read_ok = true then
         return "Line `"& lineread &"' sucessfully read from file "
            & filename &".";
      else
         return "Missing or unsuitable entry found while reading line `"
            & lineread &"' from file "& filename &".";
      end if;
   end FileReadMessage;
-------------------------------------------------------------------------------
   
   -- ### binary character => std_logic
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : binary
   -- target (VHDL signal): std_logic
   procedure GetFileEntry (
      fileentry            : inout std_logic;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable read_ok : boolean;
   begin
      -- extract next entry to obtain the value of formal variable fileentry
      read(in_line, fileentry, read_ok);  -- binary read
      if not read_ok then
         report FileReadMessage(filename, read_ok, in_line_tmp.all) severity error;
      end if;
   end GetFileEntry;

   -- ### binary string => std_logic_vector
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : binary
   -- target (VHDL signal): std_logic_vector
   procedure GetFileEntry (
      fileentry            : inout std_logic_vector;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable read_ok : boolean;
   begin
      -- extract next entry to obtain the value of formal variable fileentry
      read(in_line, fileentry, read_ok);      -- binary read
      if not read_ok then
         hread(in_line, fileentry, read_ok);  -- try hex-read
         if not read_ok then
            report FileReadMessage(filename, read_ok, in_line_tmp.all) severity error;
         end if;
      end if;
   end GetFileEntry;

   -- ### binary string => unsigned
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : binary
   -- target (VHDL signal): unsigned
   procedure GetFileEntry (
      val                  : inout unsigned;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable read_ok   : boolean;
      variable fileentry : std_logic_vector(val'high downto val'low);
   begin
      -- extract next entry to obtain the value of formal variable fileentry
      read(in_line, fileentry, read_ok);      -- binary read
      if not read_ok then
         hread(in_line, fileentry, read_ok);  -- try hex-read
         if not read_ok then
            report FileReadMessage(filename, read_ok, in_line_tmp.all) severity error;
         end if;
      end if;
      val := unsigned(fileentry);
   end GetFileEntry;

   -- ### binary string => signed
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : binary
   -- target (VHDL signal): signed
   procedure GetFileEntry (
      val                  : inout signed;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable read_ok   : boolean;
      variable fileentry : std_logic_vector(val'high downto val'low);
   begin
      -- extract next entry to obtain the value of formal variable fileentry
      read(in_line, fileentry, read_ok);      -- binary read
      if not read_ok then
         hread(in_line, fileentry, read_ok);  -- try hex-read
         if not read_ok then
            report FileReadMessage(filename, read_ok, in_line_tmp.all) severity error;
         end if;
      end if;
      val := signed(fileentry);
   end GetFileEntry;
   
   -- ### unsigned integer string => unsigned
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : unsigned integer
   -- target (VHDL signal): unsigned
   procedure GetFileEntryInt2x (
      val                  : inout unsigned;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable read_ok   : boolean;
      variable fileentry : integer;
   begin
      -- extract next entry to obtain the value of formal variable fileentry
      read(in_line, fileentry, read_ok);
      if not read_ok then
         report FileReadMessage(filename, read_ok, in_line_tmp.all) severity error;
      end if;
      val := to_unsigned(fileentry, val'length);
   end GetFileEntryInt2x;

   -- ### signed/unsigned integer string => signed
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : signed/unsigned integer
   -- target (VHDL signal): signed
   procedure GetFileEntryInt2x (
      val                  : inout signed;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable read_ok   : boolean;
      variable fileentry : integer;
   begin
      -- extract next entry to obtain the value of formal variable fileentry
      read(in_line, fileentry, read_ok);
      if not read_ok then
         report FileReadMessage(filename, read_ok, in_line_tmp.all) severity error;
      end if;
      val := to_signed(fileentry, val'length);
   end GetFileEntryInt2x;

   -- ### signed/unsigned integer string => (signed =>) std_logic_vector
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : signed/unsigned integer
   -- target (VHDL signal): (signed) std_logic_vector
   procedure GetFileEntryInt2x (
      val                  : inout std_logic_vector;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable tmp_signed : signed(1 to val'length);
   begin
      -- use overloaded procedure
      GetFileEntryInt2x(tmp_signed, in_line, in_line_tmp, filename);
      val := std_logic_vector(tmp_signed);
   end GetFileEntryInt2x;

   -- ### unsigned integer string => (unsigned =>) std_logic_vector
   -- purpose: get one entry from the stimuli or expected responses file
   -- source (file entry) : unsigned integer
   -- target (VHDL signal): (unsigned) std_logic_vector
   procedure GetFileEntryUInt2x (
      val                  : inout std_logic_vector;
      in_line, in_line_tmp : inout line;
      filename             :       string)
   is
      variable tmp_unsigned : unsigned(1 to val'length);
   begin
      -- use overloaded procedure
      GetFileEntryInt2x(tmp_unsigned, in_line, in_line_tmp, filename);
      val := std_logic_vector(tmp_unsigned);
   end GetFileEntryUInt2x;
-------------------------------------------------------------------------------

   -- purpose: check to what extent actual and expected responses match,
   --    return a grade and update failure account record accordingly.
   procedure CheckValue (
      actresp, expresp : in    std_logic_vector;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype)
   is
      type bitwise_matchtable is array (std_logic, std_logic) of respmatchtype;
      constant check_bitwise : bitwise_matchtable := (
      --     ---------------------------------------------------------
      -- exp |  U    X    0    1    Z    W    L    H    -        |act|  
      --     ---------------------------------------------------------
             ( mok, mlf, mlf, mlf, mlf, mlf, mlf, mlf, mne ), -- | U |
             ( mlf, mok, mlf, mlf, mlf, msf, mlf, mlf, mne ), -- | X |
             ( mlf, mlf, mok, mlf, mlf, mlf, msf, mlf, mne ), -- | 0 |
             ( mlf, mlf, mlf, mok, mlf, mlf, mlf, msf, mne ), -- | 1 |
             ( mlf, mlf, mlf, mlf, mok, mlf, mlf, mlf, mne ), -- | Z |
             ( mlf, msf, mlf, mlf, mlf, mok, mlf, mlf, mne ), -- | W |
             ( mlf, mlf, msf, mlf, mlf, mlf, mok, mlf, mne ), -- | L |
             ( mlf, mlf, mlf, msf, mlf, mlf, mlf, mok, mne ), -- | H |
             ( mil, mil, mil, mil, mil, mil, mil, mil, mil )  -- | - |
             );   -- act is the 1st and exp the 2nd dimension

      variable bitwise_match, vectorwise_match : respmatchtype := mne;
   begin
      assert expresp'length = actresp'length and expresp'length > 0
         report " Cardinality of response does not match or is zero."
         severity warning;
      for i in expresp'range loop
         bitwise_match    := check_bitwise(actresp(i), expresp(i));
         vectorwise_match := check_vectorwise(vectorwise_match, bitwise_match);
      end loop;
      respmatch := vectorwise_match;
      case vectorwise_match is
         when mne => respaccount.numberof_mne := respaccount.numberof_mne + 1;
         when mok => respaccount.numberof_mok := respaccount.numberof_mok + 1;
         when mlf => respaccount.numberof_mlf := respaccount.numberof_mlf + 1;
         when msf => respaccount.numberof_msf := respaccount.numberof_msf + 1;
         when mil => respaccount.numberof_mil := respaccount.numberof_mil + 1;
      end case;
      respaccount.numberof_mch := respaccount.numberof_mch + 1;
   end CheckValue;
   
   -- purpose: above procedure overloaded for scalars rather than vectors.
   procedure CheckValue (
      actresp, expresp : in    std_logic;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype)
   is
      variable actrespvector, exprespvector : std_logic_vector(0 to 0);
   begin
      actrespvector(0) := actresp; exprespvector(0) := expresp;
      CheckValue(actrespvector, exprespvector, respmatch, respaccount);
   end CheckValue;

   -- purpose: above procedure overloaded for unsigned rather than std_logic_vector
   procedure CheckValue (
      actresp, expresp : in    unsigned;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype)
   is
      variable actrespvector, exprespvector : std_logic_vector(actresp'high downto actresp'low);
   begin
      actrespvector := std_logic_vector(actresp);
      exprespvector := std_logic_vector(expresp);
      CheckValue(actrespvector, exprespvector, respmatch, respaccount);
   end CheckValue;

   -- purpose: above procedure overloaded for signed rather than std_logic_vector
   procedure CheckValue (
      actresp, expresp : in    signed;
      respmatch        : out   respmatchtype;
      respaccount      : inout respaccounttype)
   is
      variable actrespvector, exprespvector : std_logic_vector(actresp'high downto actresp'low);
   begin
      actrespvector := std_logic_vector(actresp);
      exprespvector := std_logic_vector(expresp);
      CheckValue(actrespvector, exprespvector, respmatch, respaccount);
   end CheckValue;
-------------------------------------------------------------------------------

   -- purpose: summarize simulation report and write it to report file.
   procedure PutSimulationReportSummary
      (file simreptfile : text; respaccount : in respaccounttype)
   is
      variable out_line : line;
   begin
      write(out_line, string'(" "));
      writeline(simreptfile, out_line);
      write(out_line, string'("====== Simulation Report Summary ==============================="));
      writeline(simreptfile, out_line);
      write(out_line, string'(" Total of responses: "));
      write(out_line, respaccount.numberof_mch);
      writeline(simreptfile, out_line);
      write(out_line, string'(" "));
      writeline(simreptfile, out_line);
      write(out_line, string'(" "));
      write(out_line, respaccount.numberof_mch - respaccount.numberof_mne);
      write(out_line, string'(" responses have been checked, out of which"));
      writeline(simreptfile, out_line);
      write(out_line, string'("   "));
      write(out_line, respaccount.numberof_mok);
      write(out_line, string'(" matched expectations"));
      writeline(simreptfile, out_line);
      write(out_line, string'("   "));
      write(out_line, respaccount.numberof_mlf + respaccount.numberof_msf + respaccount.numberof_mil);
      write(out_line, string'(" didn't match expectations"));
      writeline(simreptfile, out_line);
      write(out_line, string'("      "));
      write(out_line, respaccount.numberof_mlf);
      write(out_line, string'(" responses failed logically (those with a 'l')"));
      writeline(simreptfile, out_line);
      write(out_line, string'("      "));
      write(out_line, respaccount.numberof_msf);
      write(out_line, string'(" failed in drive strength (those with a 's')"));
      writeline(simreptfile, out_line);
      write(out_line, string'("      "));
      write(out_line, respaccount.numberof_mil);
      write(out_line, string'(" had an illegal logic value (those with a 'i')"));
      writeline(simreptfile, out_line);
      write(out_line, string'(" "));
      writeline(simreptfile, out_line);
      write(out_line, string'(" "));
      write(out_line, respaccount.numberof_mne);
      write(out_line, string'(" responses haven't been checked (those with a '-'), because"));
      writeline(simreptfile, out_line);
      write(out_line, string'(" "));
      write(out_line, string'("expected responses are unavailable or given as "&'"'&"don't care"&'"'&"."));
      writeline(simreptfile, out_line);
   end PutSimulationReportSummary;
-------------------------------------------------------------------------------

   -- purpose: overload write method for unsigned
   procedure write (outline : inout line; val : in unsigned) is
   begin
      write(outline, std_logic_vector(val));
   end write;

   -- purpose: overload write method for signed
   procedure write (outline : inout line; val : in signed) is
   begin
      write(outline, std_logic_vector(val));
   end write;
-------------------------------------------------------------------------------

   -- purpose: generate binary random vectors of parametrized word width that 
   --    should be uniformly distributed over interval [0,2**randvectwidth-1].
   -- limitation: mantissa of VHDL type reals has 23bits, so randvectwidth 
   --    must not exceed this value as outcome is otherwise uncertain.
   -- note: state variables of procedure Uniform must be kept within the 
   --    calling process because variables in a subprogram do not persist.
   -- findings: repeated calls of procedure ieee.math_real.Uniform with
   --    identical seeds indeed result in identical pseudo random numbers.
   procedure GenerateRandomVector (
      randvectwidth        : in    natural;
      statevar1, statevar2 : inout integer;
      randvect             : out   std_logic_vector)  -- unconstrained array type
   is
      -- upperbound is (2.0**randvectwidth)-1.0, spreadbound is 1.0 more
      constant spreadbound            : real    := 2.0**randvectwidth;
      variable randreal01             : real    := 0.0;
      variable randscaled, randtruncd : real    := 0.0;
      variable randinteger            : integer := 0;
   begin
      -- obtain a random real in the open interval ]0,1[
      Uniform(statevar1, statevar2, randreal01);
      -- scale open interval ]0,1[ to open interval ]0,spreadbound[
      randscaled  := randreal01*spreadbound;
      -- truncate to next smaller integer (still of type real, though)
      -- in the closed interval [0,upperbound]
      randtruncd  := Trunc(randscaled);
      -- convert to a binary vector
      randinteger := integer(randtruncd);
      randvect    := std_logic_vector(to_unsigned(randinteger, randvectwidth));
   end GenerateRandomVector;

   -- purpose: generate a random bit with some given probability of being '1'.
   -- note: state variables of procedure Uniform must be kept within the 
   --    calling process because variables in a subprogram do not persist.
   -- findings: repeated calls of procedure ieee.math_real.Uniform with
   --    identical seeds indeed result in identical pseudo random numbers.
   procedure GenerateRandomBit (
      probability_of_1     : in    real;
      statevar1, statevar2 : inout integer;
      randbit              : out   std_logic) 
   is
      variable randreal01 : real := 0.0;
   begin
      -- obtain a random real in the open interval ]0,1[
      Uniform(statevar1, statevar2, randreal01);
      -- set randbit according to threshold
      if (probability_of_1 > randreal01) then
         randbit := '1';
      else
         randbit := '0';
      end if;
   end GenerateRandomBit;

   -- purpose: generate random integer in the range [min, max], both included.
   -- note: state variables of procedure Uniform must be kept within the 
   --    calling process because variables in a subprogram do not persist.
   -- findings: repeated calls of procedure ieee.math_real.Uniform with
   --    identical seeds indeed result in identical pseudo random numbers.
   procedure GenerateRandomInteger (
      min, max             : in    integer;
      statevar1, statevar2 : inout integer;
      randinteger          : out   integer)  
   is
      variable randreal01  : real := 0.0;
      constant spreadbound : real := real(max-min+1);
      variable randscaled  : real := 0.0;
   begin
      -- obtain a random real in the open interval ]0,1[
      Uniform(statevar1, statevar2, randreal01);
      -- scale open interval ]0,1[ to open interval ]0,spreadbound[
      randscaled  := randreal01*spreadbound;
      -- truncate to next smaller integer in the closed interval [0,max-min] and
      -- add the mininum value to get an integer in the closed interval [min,max]
      randinteger := integer(Trunc(randscaled)) + min;
   end GenerateRandomInteger;


end package body simulstuff;
