connect -url tcp:127.0.0.1:3121
source C:/Projects/Casioproject/Casio/Casio.sdk/Casio_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo 210279574526A"} -index 0
loadhw C:/Projects/Casioproject/Casio/Casio.sdk/Casio_wrapper_hw_platform_0/system.hdf
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo 210279574526A"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zybo 210279574526A"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zybo 210279574526A"} -index 0
dow C:/Projects/Casioproject/Casio/Casio.sdk/LED_Test/Debug/LED_Test.elf
bpadd -addr &main
