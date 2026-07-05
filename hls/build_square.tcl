set PROJ_NAME VitisRho_square
source setup_common.tcl
 
add_files -tb gf_field_tb.cpp -cflags "-I../RhoPollard_lib/include -lgmp"
 
set_top gf163_square
csim_design   -ldflags "-lgmp"
csynth_design
