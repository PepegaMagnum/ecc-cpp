set PROJ_NAME VitisRho_inv
source setup_common.tcl
 
add_files -tb gf_inv_tb.cpp -cflags "-I../RhoPollard_lib/include -lgmp"
 
set_top gf163_inv
csim_design   -ldflags "-lgmp"
csynth_design
