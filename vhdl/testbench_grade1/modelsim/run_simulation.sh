#!/bin/sh

# library name
LIB=work

# Modelsim version
VER=6.3b

# remove old simulation report file
rm ../simvectors/simrept.asc

# start simulation
vsim-$VER -lib $LIB -do "do ./wave.do; run -all" multtb1
