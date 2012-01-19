onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /multtb1pkg/clkxc
add wave -noupdate -format Logic /simulstuff/endofsimxs
add wave -noupdate -format Literal -expand /multtb1pkg/stimulirecxd
add wave -noupdate -format Literal -expand /multtb1pkg/actresponserecxd
add wave -noupdate -format Literal -expand /multtb1pkg/expresponserecxd
TreeUpdate [SetDefaultTree]
configure wave -namecolwidth 168
configure wave -valuecolwidth 40
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
update
