-- Mission: See associated testbench file.
-- Functionality: The following architecture describes a shift add multiplier
--   for unsigned numbers. The multiplier takes two W-bit input values. The
--   2W-bit wide result is put out serially (LSB first).
-- Platform: Modelsim 6.0
-- Company: Microelectronics Design Center, ETH Zurich
-- Author: Thomas Kuch
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
-------------------------------------------------------------------------------

entity ShiftAddMult is
  generic (
    width          : natural := 8 );  -- word width of the two inputs
  port (
    ClkxCI         : in  std_logic;
    RstxRBI        : in  std_logic;
    StartCalcxSI   : in  std_logic;
    InputAxDI      : in  std_logic_vector(width-1 downto 0);
    InputBxDI      : in  std_logic_vector(width-1 downto 0);
    OutputxDO      : out std_logic;
    OutputValidxSO : out std_logic );
end ShiftAddMult;

-------------------------------------------------------------------------------

architecture Behavioral of ShiftAddMult is

  signal ClearRegxS   : std_logic;
  signal InputRegEnxS : std_logic;
  signal ShiftEnxS    : std_logic;

  signal InputAxDP    : unsigned(2*width-1 downto 0);
  signal InputBxDP    : unsigned(width-1 downto 0);

  signal Summand1xD : unsigned(width-1 downto 0);
  signal Summand2xD : unsigned(width-1 downto 0);
  signal SumxD      : unsigned(width downto 0);

  type statetype is (idle, newData, calculate);
  signal StatexDP, StatexDN : statetype;

  signal CounterxDP, CounterxDN : natural range 0 to 2*width -1;


begin  -- Behavioral
  
  -- Input registers
  -----------------------------------------------------------------------------

  -- Shift register for input A
  InpAReg: process (ClkxCI, RstxRBI)
  begin
    if RstxRBI = '0' then             -- asynchronous reset (active low)
      InputAxDP <= (others => '0');
    elsif ClkxCI'event and ClkxCI = '1' then  -- rising clock edge
      if InputRegEnxS = '1' then
        InputAxDP(width-1 downto 0) <= unsigned(InputAxDI);
      elsif ClearRegxS = '1' then
        InputAxDP <= (others => '0');
      elsif ShiftEnxS = '1' then
        InputAxDP <= InputAxDP srl 1;   -- the MSB is padded with 0
      end if;
    end if; 
  end process InpAReg;

  -- Register for input B
  InpBReg: process (ClkxCI, RstxRBI)
  begin
    if RstxRBI = '0' then             -- asynchronous reset (active low)
      InputBxDP <= (others => '0');
    elsif ClkxCI'event and ClkxCI = '1' then  -- rising clock edge
      if InputRegEnxS = '1' then
        InputBxDP <= unsigned(InputBxDI);
      elsif ClearRegxS = '1' then
        InputBxDP <= (others => '0'); 
      end if;  
    end if; 
  end process InpBReg;

  
  -- Bitwise multiplication of one bit of input A with the whole input B
  -----------------------------------------------------------------------------

  AndGate: process (InputAxDP, InputBxDP)
  begin
    for i in width-1 downto 0 loop
      Summand1xD(i) <=  InputAxDP(0) and InputBxDP(i);    
    end loop;
  end process AndGate; 


  -- Adder
  ----------------------------------------------------------------------------- 
  SumxD <= resize(Summand1xD, width+1) + resize(Summand2xD, width+1);
 

  -- Register for summation
  -----------------------------------------------------------------------------
  
  SumReg: process (ClkxCI, RstxRBI)
  begin
    if RstxRBI = '0' then             -- asynchronous reset (active low)
      Summand2xD <= (others => '0');
    elsif ClkxCI'event and ClkxCI = '1' then  -- rising clock edge
      if ClearRegxS = '1' then
        Summand2xD <= (others => '0');
      else
        Summand2xD <= SumxD(width downto 1);
      end if;  
    end if; 
  end process SumReg;


  -- Output assignment
  -----------------------------------------------------------------------------
  OutputxDO <= std_logic(SumxD(0));
  

  -- Controller FSM
  -----------------------------------------------------------------------------
  
  -- Combinational process
  FSMcomb: process (StatexDP, StartCalcxSI, CounterxDP)
  begin
    -- default assignments
    InputRegEnxS   <= '0';
    ShiftEnxS      <= '0';
    OutputValidxSO <= '0';
    ClearRegxS     <= '0';
    StatexDN       <= StatexDP;
    CounterxDN     <= CounterxDP;

    -- non-default transitions and assignments
    case StatexDP is 
      when idle =>  
        ClearRegxS <= '1';
        if StartCalcxSI = '1' then
          StatexDN <= newData;
        end if;
        
      when newData =>
        InputRegEnxS <= '1';
        StatexDN     <= calculate;
        
      when calculate =>
        ShiftEnxS      <= '1';
        OutputValidxSO <= '1';
        if CounterxDP = 2*width - 1 then
          CounterxDN <= 0;
          StatexDN   <= idle;
        else
           CounterxDN <= CounterxDP + 1;
        end if; 
   
      when others =>
        StatexDN <= idle;
    end case;
  end process FSMcomb;

  -- State register
  FSMseq: process (ClkxCI, RstxRBI)
  begin
    if RstxRBI = '0' then             -- asynchronous reset (active low)
      StatexDP   <= idle;
      CounterxDP <= 0;
    elsif ClkxCI'event and ClkxCI = '1' then  -- rising clock edge
      StatexDP   <= StatexDN;
      CounterxDP <= CounterxDN;
    end if;
  end process FSMseq;

end architecture Behavioral; -- of ShiftAddMult
