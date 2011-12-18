-------------------------------------------------------------------------------
-- dcr_timebase_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library dcr_timebase_v1_00_b;
use dcr_timebase_v1_00_b.all;

entity dcr_timebase_0_wrapper is
  port (
    i_clk : in std_logic;
    i_reset : in std_logic;
    o_dcrAck : out std_logic;
    o_dcrDBus : out std_logic_vector(0 to 31);
    i_dcrABus : in std_logic_vector(0 to 9);
    i_dcrDBus : in std_logic_vector(0 to 31);
    i_dcrRead : in std_logic;
    i_dcrWrite : in std_logic;
    o_timeBase : out std_logic_vector(0 to 31);
    o_irq : out std_logic
  );
end dcr_timebase_0_wrapper;

architecture STRUCTURE of dcr_timebase_0_wrapper is

  component dcr_timebase is
    generic (
      C_FAMILY : string;
      C_DCR_BASEADDR : std_logic_vector;
      C_DCR_HIGHADDR : std_logic_vector;
      C_DCR_AWIDTH : integer;
      C_DCR_DWIDTH : integer
    );
    port (
      i_clk : in std_logic;
      i_reset : in std_logic;
      o_dcrAck : out std_logic;
      o_dcrDBus : out std_logic_vector(0 to C_DCR_DWIDTH-1);
      i_dcrABus : in std_logic_vector(0 to C_DCR_AWIDTH-1);
      i_dcrDBus : in std_logic_vector(0 to C_DCR_DWIDTH-1);
      i_dcrRead : in std_logic;
      i_dcrWrite : in std_logic;
      o_timeBase : out std_logic_vector(0 to C_DCR_DWIDTH-1);
      o_irq : out std_logic
    );
  end component;

begin

  dcr_timebase_0 : dcr_timebase
    generic map (
      C_FAMILY => "virtex6",
      C_DCR_BASEADDR => B"0010000000",
      C_DCR_HIGHADDR => B"0010000011",
      C_DCR_AWIDTH => 10,
      C_DCR_DWIDTH => 32
    )
    port map (
      i_clk => i_clk,
      i_reset => i_reset,
      o_dcrAck => o_dcrAck,
      o_dcrDBus => o_dcrDBus,
      i_dcrABus => i_dcrABus,
      i_dcrDBus => i_dcrDBus,
      i_dcrRead => i_dcrRead,
      i_dcrWrite => i_dcrWrite,
      o_timeBase => o_timeBase,
      o_irq => o_irq
    );

end architecture STRUCTURE;

