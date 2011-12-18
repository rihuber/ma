-------------------------------------------------------------------------------
-- hw_task_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library hw_task_v1_01_b;
use hw_task_v1_01_b.all;

entity hw_task_0_wrapper is
  port (
    clk : in std_logic;
    reset : in std_logic;
    i_osif_flat : in std_logic_vector(0 to 46);
    o_osif_flat : out std_logic_vector(0 to 50);
    i_threadClk : in std_logic;
    i_burstAddr : in std_logic_vector(0 to 13);
    i_burstData : in std_logic_vector(0 to 63);
    o_burstData : out std_logic_vector(0 to 63);
    i_burstWE : in std_logic;
    i_burstBE : in std_logic_vector(0 to 7)
  );
end hw_task_0_wrapper;

architecture STRUCTURE of hw_task_0_wrapper is

  component hw_task is
    generic (
      C_BUS_BURST_AWIDTH : INTEGER;
      C_BUS_BURST_DWIDTH : INTEGER;
      C_TASK_BURST_AWIDTH : INTEGER;
      C_TASK_BURST_DWIDTH : INTEGER;
      C_REGISTER_OSIF_PORTS : INTEGER;
      C_DEDICATED_CLK : INTEGER
    );
    port (
      clk : in std_logic;
      reset : in std_logic;
      i_osif_flat : in std_logic_vector(0 to 46);
      o_osif_flat : out std_logic_vector(0 to 50);
      i_threadClk : in std_logic;
      i_burstAddr : in std_logic_vector(0 to C_BUS_BURST_AWIDTH-1);
      i_burstData : in std_logic_vector(0 to C_BUS_BURST_DWIDTH-1);
      o_burstData : out std_logic_vector(0 to C_BUS_BURST_DWIDTH-1);
      i_burstWE : in std_logic;
      i_burstBE : in std_logic_vector(0 to C_BUS_BURST_DWIDTH/8-1)
    );
  end component;

begin

  hw_task_0 : hw_task
    generic map (
      C_BUS_BURST_AWIDTH => 14,
      C_BUS_BURST_DWIDTH => 64,
      C_TASK_BURST_AWIDTH => 12,
      C_TASK_BURST_DWIDTH => 32,
      C_REGISTER_OSIF_PORTS => 0,
      C_DEDICATED_CLK => 0
    )
    port map (
      clk => clk,
      reset => reset,
      i_osif_flat => i_osif_flat,
      o_osif_flat => o_osif_flat,
      i_threadClk => i_threadClk,
      i_burstAddr => i_burstAddr,
      i_burstData => i_burstData,
      o_burstData => o_burstData,
      i_burstWE => i_burstWE,
      i_burstBE => i_burstBE
    );

end architecture STRUCTURE;

