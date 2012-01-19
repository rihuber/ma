-- Mission: See associated testbench file.
-- Functionality: This entity contains the functional reference of the
-- testbench. The reference picks up expected responses from an ASCII file.
-- Platform: Modelsim 6.0
-- Company: Microelectronics Design Center, ETH Zurich.
-- Authors: Thomas Kuch
-------------------------------------------------------------------------------

use std.textio.all;
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_textio.all;
use ieee.numeric_std.all;
use work.simulstuff.all;
use work.multTb1Pkg.all;

-- This architecture picks up the expected responses from a file
-- (the architecture uses the same entity as the MUT)
-------------------------------------------------------------------------------
architecture Pickup of ShiftAddMult is

  -- the file is opened implicitly right here
  file exprespfile : text open read_mode is expresp_filename;

begin
  ExpResPickup : process
    variable ResponsexD : responseRecordType;
  begin

    PickupLoop : loop  
      wait until (ClkxCI'event and ClkxCI = '1') or EndOfSimxS = true;

      -- leave the loop if there are no more stimuli left
      exit PickupLoop when EndOfSimxS = true;
      
      -- update expected response from file
      ResponsexD := GetExpectedResponseRecord(exprespfile);
      OutputxDO      <= ResponsexD.OutputxD;
      OutputValidxSO <= ResponsexD.OutputValidxS;
      
   end loop PickupLoop;

    file_close(exprespfile);  -- close the file
    wait;
  end process ExpResPickup;

end architecture Pickup;  -- of ShiftAddMult
