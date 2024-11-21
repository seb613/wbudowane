set_global_assignment -entity "LAB01SYSWBUDOWANE" -library "lib_LAB01SYSWBUDOWANE" -name IP_TOOL_NAME "Qsys"
set_global_assignment -entity "LAB01SYSWBUDOWANE" -library "lib_LAB01SYSWBUDOWANE" -name IP_TOOL_VERSION "17.1"
set_global_assignment -entity "LAB01SYSWBUDOWANE" -library "lib_LAB01SYSWBUDOWANE" -name IP_TOOL_ENV "Qsys"
set_global_assignment -library "lib_LAB01SYSWBUDOWANE" -name SPD_FILE [file join $::quartus(sip_path) "../LAB01SYSWBUDOWANE.spd"]
set_global_assignment -library "lib_LAB01SYSWBUDOWANE" -name MISC_FILE [file join $::quartus(sip_path) "../LAB01SYSWBUDOWANE.qsys"]

set_global_assignment -library "lib_LAB01SYSWBUDOWANE" -name MISC_FILE [file join $::quartus(sip_path) "LAB01SYSWBUDOWANE.vhd"]
set_global_assignment -library "lib_LAB01SYSWBUDOWANE" -name MISC_FILE [file join $::quartus(sip_path) "lab01syswbudowane_rst_controller.vhd"]
set_global_assignment -library "lib_LAB01SYSWBUDOWANE" -name MISC_FILE [file join $::quartus(sip_path) "lab01syswbudowane_rst_controller_001.vhd"]
set_global_assignment -library "lib_LAB01SYSWBUDOWANE" -name MISC_FILE [file join $::quartus(sip_path) "lab01syswbudowane_rst_controller_002.vhd"]
