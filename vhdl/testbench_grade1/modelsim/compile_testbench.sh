#!/bin/sh

# library name
LIB=work

# Modelsim version
VER=6.4

# delete and recreate library 
rm -rf $LIB
vlib-$VER $LIB

# compile files
vcom-$VER -work $LIB ../sourcecode/simulstuff.vhd
vcom-$VER -work $LIB ../sourcecode/multtb1pkg.vhd
vcom-$VER -work $LIB ../sourcecode/shiftaddmult.vhd
vcom-$VER -work $LIB ../sourcecode/multref.vhd
vcom-$VER -work $LIB ../sourcecode/multtb1.vhd

