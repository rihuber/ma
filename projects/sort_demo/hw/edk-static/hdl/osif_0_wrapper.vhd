-------------------------------------------------------------------------------
-- osif_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library xps_osif_v2_01_a;
use xps_osif_v2_01_a.all;

entity osif_0_wrapper is
  port (
    MPLB_Clk : in std_logic;
    MPLB_Rst : in std_logic;
    MD_error : out std_logic;
    M_request : out std_logic;
    M_priority : out std_logic_vector(0 to 1);
    M_busLock : out std_logic;
    M_RNW : out std_logic;
    M_BE : out std_logic_vector(0 to 7);
    M_MSize : out std_logic_vector(0 to 1);
    M_size : out std_logic_vector(0 to 3);
    M_type : out std_logic_vector(0 to 2);
    M_TAttribute : out std_logic_vector(0 to 15);
    M_lockErr : out std_logic;
    M_abort : out std_logic;
    M_UABus : out std_logic_vector(0 to 31);
    M_ABus : out std_logic_vector(0 to 31);
    M_wrDBus : out std_logic_vector(0 to 63);
    M_wrBurst : out std_logic;
    M_rdBurst : out std_logic;
    PLB_MAddrAck : in std_logic;
    PLB_MSSize : in std_logic_vector(0 to 1);
    PLB_MRearbitrate : in std_logic;
    PLB_MTimeout : in std_logic;
    PLB_MBusy : in std_logic;
    PLB_MRdErr : in std_logic;
    PLB_MWrErr : in std_logic;
    PLB_MIRQ : in std_logic;
    PLB_MRdDBus : in std_logic_vector(0 to 63);
    PLB_MRdWdAddr : in std_logic_vector(0 to 3);
    PLB_MRdDAck : in std_logic;
    PLB_MRdBTerm : in std_logic;
    PLB_MWrDAck : in std_logic;
    PLB_MWrBTerm : in std_logic;
    sys_clk : in std_logic;
    sys_reset : in std_logic;
    o_dcrAck : out std_logic;
    o_dcrDBus : out std_logic_vector(0 to 31);
    i_dcrABus : in std_logic_vector(0 to 9);
    i_dcrDBus : in std_logic_vector(0 to 31);
    i_dcrRead : in std_logic;
    i_dcrWrite : in std_logic;
    interrupt : out std_logic;
    busy : out std_logic;
    blocking : out std_logic;
    i_dcrICON : in std_logic_vector(35 downto 0);
    task_clk : out std_logic;
    task_reset : out std_logic;
    osif_os2task_vec : out std_logic_vector(0 to 46);
    osif_task2os_vec : in std_logic_vector(0 to 50);
    burstAddr : out std_logic_vector(0 to 13);
    burstWrData : out std_logic_vector(0 to 63);
    burstRdData : in std_logic_vector(0 to 63);
    burstWE : out std_logic;
    burstBE : out std_logic_vector(0 to 7);
    o_fifo_read_en : out std_logic;
    i_fifo_read_data : in std_logic_vector(0 to 31);
    i_fifo_read_ready : in std_logic;
    o_fifo_clk : out std_logic;
    o_fifo_reset : out std_logic;
    o_fifo_write_en : out std_logic;
    o_fifo_write_data : out std_logic_vector(0 to 31);
    i_fifo_write_ready : in std_logic;
    bmEnable : out std_logic
  );
end osif_0_wrapper;

architecture STRUCTURE of osif_0_wrapper is

  component xps_osif is
    generic (
      C_FAMILY : STRING;
      C_MPLB_AWIDTH : INTEGER;
      C_MPLB_DWIDTH : INTEGER;
      C_MPLB_NATIVE_DWIDTH : INTEGER;
      C_MPLB_P2P : INTEGER;
      C_MPLB_SMALLEST_SLAVE : INTEGER;
      C_MPLB_CLK_PERIOD_PS : INTEGER;
      C_BURST_AWIDTH : INTEGER;
      C_FIFO_DWIDTH : INTEGER;
      C_DCR_BASEADDR : std_logic_vector;
      C_DCR_HIGHADDR : std_logic_vector;
      C_DCR_AWIDTH : integer;
      C_DCR_DWIDTH : integer;
      C_DCR_ILA : INTEGER
    );
    port (
      MPLB_Clk : in std_logic;
      MPLB_Rst : in std_logic;
      MD_error : out std_logic;
      M_request : out std_logic;
      M_priority : out std_logic_vector(0 to 1);
      M_busLock : out std_logic;
      M_RNW : out std_logic;
      M_BE : out std_logic_vector(0 to ((C_MPLB_DWIDTH/8)-1));
      M_MSize : out std_logic_vector(0 to 1);
      M_size : out std_logic_vector(0 to 3);
      M_type : out std_logic_vector(0 to 2);
      M_TAttribute : out std_logic_vector(0 to 15);
      M_lockErr : out std_logic;
      M_abort : out std_logic;
      M_UABus : out std_logic_vector(0 to 31);
      M_ABus : out std_logic_vector(0 to 31);
      M_wrDBus : out std_logic_vector(0 to (C_MPLB_DWIDTH-1));
      M_wrBurst : out std_logic;
      M_rdBurst : out std_logic;
      PLB_MAddrAck : in std_logic;
      PLB_MSSize : in std_logic_vector(0 to 1);
      PLB_MRearbitrate : in std_logic;
      PLB_MTimeout : in std_logic;
      PLB_MBusy : in std_logic;
      PLB_MRdErr : in std_logic;
      PLB_MWrErr : in std_logic;
      PLB_MIRQ : in std_logic;
      PLB_MRdDBus : in std_logic_vector(0 to (C_MPLB_DWIDTH-1));
      PLB_MRdWdAddr : in std_logic_vector(0 to 3);
      PLB_MRdDAck : in std_logic;
      PLB_MRdBTerm : in std_logic;
      PLB_MWrDAck : in std_logic;
      PLB_MWrBTerm : in std_logic;
      sys_clk : in std_logic;
      sys_reset : in std_logic;
      o_dcrAck : out std_logic;
      o_dcrDBus : out std_logic_vector(0 to C_DCR_DWIDTH-1);
      i_dcrABus : in std_logic_vector(0 to C_DCR_AWIDTH-1);
      i_dcrDBus : in std_logic_vector(0 to C_DCR_DWIDTH-1);
      i_dcrRead : in std_logic;
      i_dcrWrite : in std_logic;
      interrupt : out std_logic;
      busy : out std_logic;
      blocking : out std_logic;
      i_dcrICON : in std_logic_vector(35 downto 0);
      task_clk : out std_logic;
      task_reset : out std_logic;
      osif_os2task_vec : out std_logic_vector(0 to 46);
      osif_task2os_vec : in std_logic_vector(0 to 50);
      burstAddr : out std_logic_vector(0 to C_BURST_AWIDTH-1);
      burstWrData : out std_logic_vector(0 to C_MPLB_NATIVE_DWIDTH-1);
      burstRdData : in std_logic_vector(0 to C_MPLB_NATIVE_DWIDTH-1);
      burstWE : out std_logic;
      burstBE : out std_logic_vector(0 to C_MPLB_NATIVE_DWIDTH/8-1);
      o_fifo_read_en : out std_logic;
      i_fifo_read_data : in std_logic_vector(0 to C_FIFO_DWIDTH-1);
      i_fifo_read_ready : in std_logic;
      o_fifo_clk : out std_logic;
      o_fifo_reset : out std_logic;
      o_fifo_write_en : out std_logic;
      o_fifo_write_data : out std_logic_vector(0 to C_FIFO_DWIDTH-1);
      i_fifo_write_ready : in std_logic;
      bmEnable : out std_logic
    );
  end component;

begin

  osif_0 : xps_osif
    generic map (
      C_FAMILY => "virtex6",
      C_MPLB_AWIDTH => 32,
      C_MPLB_DWIDTH => 64,
      C_MPLB_NATIVE_DWIDTH => 64,
      C_MPLB_P2P => 0,
      C_MPLB_SMALLEST_SLAVE => 32,
      C_MPLB_CLK_PERIOD_PS => 10000,
      C_BURST_AWIDTH => 14,
      C_FIFO_DWIDTH => 32,
      C_DCR_BASEADDR => B"0000000000",
      C_DCR_HIGHADDR => B"0000000011",
      C_DCR_AWIDTH => 10,
      C_DCR_DWIDTH => 32,
      C_DCR_ILA => 0
    )
    port map (
      MPLB_Clk => MPLB_Clk,
      MPLB_Rst => MPLB_Rst,
      MD_error => MD_error,
      M_request => M_request,
      M_priority => M_priority,
      M_busLock => M_busLock,
      M_RNW => M_RNW,
      M_BE => M_BE,
      M_MSize => M_MSize,
      M_size => M_size,
      M_type => M_type,
      M_TAttribute => M_TAttribute,
      M_lockErr => M_lockErr,
      M_abort => M_abort,
      M_UABus => M_UABus,
      M_ABus => M_ABus,
      M_wrDBus => M_wrDBus,
      M_wrBurst => M_wrBurst,
      M_rdBurst => M_rdBurst,
      PLB_MAddrAck => PLB_MAddrAck,
      PLB_MSSize => PLB_MSSize,
      PLB_MRearbitrate => PLB_MRearbitrate,
      PLB_MTimeout => PLB_MTimeout,
      PLB_MBusy => PLB_MBusy,
      PLB_MRdErr => PLB_MRdErr,
      PLB_MWrErr => PLB_MWrErr,
      PLB_MIRQ => PLB_MIRQ,
      PLB_MRdDBus => PLB_MRdDBus,
      PLB_MRdWdAddr => PLB_MRdWdAddr,
      PLB_MRdDAck => PLB_MRdDAck,
      PLB_MRdBTerm => PLB_MRdBTerm,
      PLB_MWrDAck => PLB_MWrDAck,
      PLB_MWrBTerm => PLB_MWrBTerm,
      sys_clk => sys_clk,
      sys_reset => sys_reset,
      o_dcrAck => o_dcrAck,
      o_dcrDBus => o_dcrDBus,
      i_dcrABus => i_dcrABus,
      i_dcrDBus => i_dcrDBus,
      i_dcrRead => i_dcrRead,
      i_dcrWrite => i_dcrWrite,
      interrupt => interrupt,
      busy => busy,
      blocking => blocking,
      i_dcrICON => i_dcrICON,
      task_clk => task_clk,
      task_reset => task_reset,
      osif_os2task_vec => osif_os2task_vec,
      osif_task2os_vec => osif_task2os_vec,
      burstAddr => burstAddr,
      burstWrData => burstWrData,
      burstRdData => burstRdData,
      burstWE => burstWE,
      burstBE => burstBE,
      o_fifo_read_en => o_fifo_read_en,
      i_fifo_read_data => i_fifo_read_data,
      i_fifo_read_ready => i_fifo_read_ready,
      o_fifo_clk => o_fifo_clk,
      o_fifo_reset => o_fifo_reset,
      o_fifo_write_en => o_fifo_write_en,
      o_fifo_write_data => o_fifo_write_data,
      i_fifo_write_ready => i_fifo_write_ready,
      bmEnable => bmEnable
    );

end architecture STRUCTURE;

