#!/bin/sh

xbash -q -c "cd ../; /usr/bin/make -f system.make program; exit;"

cp -pf ../implementation/download.bit .

cp -pf ../bootloader/executable.elf .
cp -pf ../hello_uart/hello_uart.elf .
cp -pf ../hello_gpio/hello_gpio.elf .
cp -pf ../hello_timer/hello_timer.elf .
cp -pf ../hello_flash/hello_flash.elf .
cp -pf ../hello_iic/hello_iic.elf .
cp -pf ../hello_temac/hello_temac.elf .
cp -pf ../hello_switch/hello_switch.elf .
cp -pf ../hello_mem/hello_mem.elf .
cp -pf ../hello_mon/hello_mon.elf .
cp -pf ../hello_button/hello_button.elf .
cp -pf ../hello_lcd/hello_lcd.elf .
cp -pf ../hello_sysace/hello_sysace.elf .
cp -pf ../colorbar/colorbar.elf .

mb-objcopy -O srec hello_uart.elf   cf_image/bist/1.rec
mb-objcopy -O srec hello_gpio.elf   cf_image/bist/2.rec
mb-objcopy -O srec hello_timer.elf  cf_image/bist/3.rec
mb-objcopy -O srec hello_flash.elf  cf_image/bist/4.rec
mb-objcopy -O srec hello_iic.elf    cf_image/bist/5.rec
mb-objcopy -O srec hello_temac.elf  cf_image/bist/6.rec
mb-objcopy -O srec hello_switch.elf cf_image/bist/7.rec
mb-objcopy -O srec hello_mem.elf    cf_image/bist/8.rec
mb-objcopy -O srec hello_mon.elf    cf_image/bist/9.rec
mb-objcopy -O srec hello_button.elf cf_image/bist/A.rec
mb-objcopy -O srec hello_lcd.elf    cf_image/bist/B.rec
mb-objcopy -O srec hello_sysace.elf cf_image/bist/C.rec
mb-objcopy -O srec colorbar.elf     cf_image/bist/D.rec

xmd -tcl genace.tcl -hw download.bit -ace ./cf_image/XILINX/cfg0/ml605_bist.ace -board ml605 -target mdm

rm -f *.svf
rm -f cf_image/XILINX/cfg0/*.svf
