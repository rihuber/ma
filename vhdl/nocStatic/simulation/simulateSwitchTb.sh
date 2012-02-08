#!/bin/sh

# library name
LIB=work

# delete and recreate library 
rm -rf $LIB
vlib $LIB

# compile files of MUT
vcom ../src/utilPkg.vhd
vcom ../src/switch/header/headerPkg.vhd
vcom ../src/switch/switchPkg.vhd
vcom ../src/switch/endOfPacket/endOfPacketDetector.vhd
vcom ../src/switch/header/headerDecoder.vhd
vcom ../src/switch/header/headerDetector.vhd
vcom ../src/switch/router/extTxPortSelect.vhd
vcom ../src/switch/router/headerFetch.vhd
vcom ../src/switch/router/headerSelect.vhd
vcom ../src/switch/router/intTxPortSelect.vhd
vcom ../src/switch/router/rxPort.vhd
vcom ../src/switch/router/txFifo.vhd
vcom ../src/switch/router/txFifoSelect.vhd
vcom ../src/switch/router/txPort.vhd
vcom ../src/switch/router/router.vhd
vcom ../src/switch/switch.vhd
vcom ../src/dummy.vhd


# compile simulation framework
vcom simulationPkg/simulationPkg.vhd

# compile testbench
vcom switchTb.vhd

# create fifos
mkdir namedPipes
mkfifo namedPipes/stimuli
mkfifo namedPipes/response

# start java model
#java -classpath ../../../java/SwitchModel/bin/ ch.rihuber.switchModel.SwitchModel stimuliFileName=namedPipes/stimuli responseFileName=namedPipes/response &

# start simulation
# vsim -do "run -all" work.switchTb
vsim work.switchTb

# remove fifos
rm -r namedPipes
